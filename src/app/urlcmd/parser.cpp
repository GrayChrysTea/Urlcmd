#include "parser.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

namespace Uc2 = UrlCmd::UrlCmd;

static const boost::regex re(
    "^(?<path>([-\\w\\d\\$\\^@\\*:%._\\+~=\"'\\\\\\/\\(\\)]+)*)"
    "(?<query>\\?([-\\w\\d\\$\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
        "[-\\w\\d\\$\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+)?"
        "(&([-\\w\\d\\$\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
        "[-\\w\\d\\$\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+))*)?"
    "(?<fragment>#([-\\w\\d\\$\\^@\\*:%._\\+~=\"'\\\\\\/\\(\\)]+)?)?");

static const boost::regex queriesRe(
    "([-\\w\\d\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
    "[-\\w\\d\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+)");

static const boost::regex intRe("^\\d+$");

static inline std::string surround(std::string s) {
    return '\"' + s + '\"';
}

static std::string replaceEscapeCodes(std::string input) {
    std::string result;
    // If 0, then parse as normal
    // If 1, then parse the first character of escape code
    // If 2, then parse the second character of escape code
    int state = 0;
    char asciiResult = 0;
    for (size_t index = 0; index < input.size(); index++) {
        char current = input[index];
        if (state == 0) {
            if (current == '%') {
                state = 1;
            } else if (current == '+') {
                result += ' ';
            } else {
                result += current;
            }
        } else if (state & 3) {
            if ('0' <= current && current <= '9') {
                asciiResult += current - '0';
            } else if ('a' <= current && current <= 'f') {
                asciiResult += current - 'a' + 10;
            } else if ('A' <= current && current <= 'F') {
                asciiResult += current - 'A' + 10;
            } else {
                throw std::string("Unexpected character in escape code: ")
                    + current
                    + " at index "
                    + std::to_string(index);
            }
            if (state == 1) {
                // Multiply by 16 to move first digit to 'tens' digit
                asciiResult <<= 4;
                state++;
            } else if (state >= 2) {
                state = 0;
                result += asciiResult;
                asciiResult = 0;
            }
        }
    }
    if (state)
        throw std::string("Unterminated escape code!");
    return result;
}

static std::string dosFormat(std::string input, int options = 0) {
    if (input.find(" ") == std::string::npos) {
        return input;
    } else {
        return surround(input);
    }
}

static std::string generateFlag(std::string flagName, int options = 0) {
    if (options & Uc2::VERBOSE) {
        std::cout << "Generating flag from: " << flagName << '\n';
        if (options & Uc2::DOSFORM) {
            std::cout << "Using the DOS format.\n";
        } else {
            std::cout << "Using the bash format.\n";
        }
    }
    if (!flagName.size()) {
        return "--";
    }
    std::string output;
    if (options & Uc2::DOSFORM) {
        output += "/";
        output += flagName;
    } else {
        output += "-";
        if (flagName.size() > 1) {
            output += "-";
        }
        output += replaceEscapeCodes(flagName);
    }
    return output;
}

Uc2::Command::Command(
    std::string command,
    int options
) : mCommand(command) {
    destructure(options);
}

int Uc2::Command::destructure(int options) {
    boost::match_results<std::string::const_iterator> match;
    if (!boost::regex_match(mCommand, match, re, boost::match_partial)) {
        if (options & Uc2::VERBOSE) {
            std::cout << "Invalid command detected!\n";
        }
        mStatus = -1;
        return 0;
    }
    if (options & Uc2::VERBOSE)
        std::cout << "\nParsing command...\n"
            << "Path: " << match["path"] << '\n'
            << "Queries: " << match["query"] << '\n'
            << "Fragment: " << match["fragment"] << '\n';
    if (!match["path"].matched) {
        if (options & Uc2::VERBOSE) {
            std::cout << "Invalid command detected!\n";
        }
        mStatus = -2;
        return 0;
    } else {
        mPath = match["path"].str();
    }
    if (match["query"].matched) {
        mQueryUnsplit = match["query"].str();
    }
    if (match["fragment"].matched) {
        mFragment = match["fragment"].str();
    }

    if (mQueryUnsplit.size() > 0 && mQueryUnsplit[0] == '?') {
        if (options & Uc2::VERBOSE) {
            std::cout << "Parsing queries...\n";
        }
        boost::smatch sm;
        std::string::const_iterator strStart = mQueryUnsplit.cbegin();
        while (boost::regex_search(
            strStart,
            mQueryUnsplit.cend(),
            sm,
            queriesRe)
        ) {
            if (options & Uc2::VERBOSE) {
                std::cout << "\t" << sm[0] << '\n';
            }
            mQueries.push_back(sm[0].str());
            strStart = sm.suffix().first;
        }
    }

    mStatus = 1;
    return 1;
}

Uc2::Command &Uc2::Command::clear(void) {
    mPath = "";
    mQueryUnsplit = "";
    mQueries.clear();
    mFragment = "";
    mStatus = 0;
    return *this;
}

int Uc2::Command::processed(void) {
    return mStatus != 0;
}

int Uc2::Command::ok(void) {
    return mStatus > 0;
}

int Uc2::Command::err(void) {
    return mStatus < 0;
}

std::string Uc2::Command::path(void) {
    return mPath;
}

std::vector<Uc2::Query> Uc2::Command::queries(void) {
    std::vector<Uc2::Query> queries;
    for (auto query : mQueries) {
        queries.push_back(query);
    }
    return queries;
}

std::string Uc2::Command::fragment(void) {
    return mFragment;
}

Uc2::Query::Query(std::string query) {
    std::size_t partition = query.find('=');
    std::string leftHand = query.substr(0, partition);
    std::string rightHand = query.substr(partition+1);
    boost::match_results<std::string::const_iterator> match;
    if (leftHand[0] == '^') {
        mQueryKind = Uc2::QueryKind::Flag;
        if (leftHand.size() <= 1) {
            throw std::string("Value Error! Flag must have an identifier.");
        }
        mLeft = leftHand.substr(1);
        if (!boost::regex_match(rightHand, match, intRe)) {
            throw "Value Error! Expected an integer but got: " + rightHand;
        } else {
            mRight = rightHand;
            mRightInt = {std::stoi(rightHand)};
        }
    } else if (leftHand[0] == '@') {
        if (leftHand.size() <= 1) {
            throw std::string(
                "Value Error! Parameter must have an identifier.");
        }
        if (leftHand[1] == '@') {
            if (leftHand.size() > 2) {
                throw std::string(
                    "Subcommand flags cannot have any characters after '@'."
                );
            }
            mQueryKind = Uc2::QueryKind::SubCommandFlag;
            mRight = rightHand;
            return;
        }
        std::string leftIdentifier = leftHand.substr(1);
        if (boost::regex_match(leftIdentifier, match, intRe)) {
            mQueryKind = Uc2::QueryKind::Positional;
            mLeft = leftIdentifier;
            mLeftInt = {std::stoi(leftIdentifier)};
            mRight = rightHand;
        } else {
            mQueryKind = Uc2::QueryKind::SubCommandOption;
            mLeft = leftIdentifier;
            mRight = rightHand;
        }
    } else {
        mQueryKind = Uc2::QueryKind::Option;
        mLeft = leftHand;
        mRight = rightHand;
    }
}

Uc2::QueryKind Uc2::Query::queryKind(void) {
    return mQueryKind;
}

std::string Uc2::Query::left(void) {
    return mLeft;
}

std::string Uc2::Query::right(void) {
    return mRight;
}

std::string Uc2::Query::format(int options) {
    if (mQueryKind == Uc2::QueryKind::Option) {
        if (options & Uc2::DOSFORM) {
            return dosFormat(generateFlag(mLeft, options))
                + ":"
                + dosFormat(replaceEscapeCodes(mRight));
        } else {
            return surround(generateFlag(mLeft, options))
                + " "
                + surround(replaceEscapeCodes(mRight));
        }
    } else if (mQueryKind == Uc2::QueryKind::Flag) {
        std::string output("");
        std::string customLeft = (options & Uc2::DOSFORM)
            ? dosFormat(generateFlag(mLeft, options))
            : surround(generateFlag(mLeft, options));
        for (int i = 0; i < mRightInt.value_or(0); i++) {
            output += customLeft;
            output += ' ';
        }
        return output;
    } else if (mQueryKind == Uc2::QueryKind::Positional) {
        return surround(replaceEscapeCodes(mRight));
    } else if (mQueryKind == Uc2::QueryKind::SubCommandFlag) {
        return surround(replaceEscapeCodes(mRight));
    } else if (mQueryKind == Uc2::QueryKind::SubCommandOption) {
        return surround(replaceEscapeCodes(mLeft))
            + " "
            + surround(replaceEscapeCodes(mRight));
    }
    throw std::string("Unrecognized QueryKind.");
}

int Uc2::Query::position(void) {
    if (mQueryKind == Uc2::QueryKind::Positional) {
        return mLeftInt.value();
    } else if (mQueryKind == Uc2::QueryKind::Flag) {
        return -1;
    } else {
        return 0;
    }
}

Uc2::Builder::Builder(Command &command, int options) {
    int queryOffset = 1;
    {
        std::string path = (options & Uc2::DOSFORM)
            ? dosFormat(replaceEscapeCodes(command.path()))
            : surround(replaceEscapeCodes(command.path()));
        if (options & Uc2::VERBOSE)
            std::cout << "Building command with path: " << path << '\n';
        mClargs.push_back(path);
    }
    {
        std::string fragment = command.fragment();
        if (fragment != "") {
            fragment = (options & Uc2::DOSFORM)
                ? dosFormat(replaceEscapeCodes(fragment.substr(1)))
                : surround(replaceEscapeCodes(fragment.substr(1)));
            if (options & Uc2::VERBOSE) {
                std::cout << "Fragment present: " << fragment << '\n';
                std::cout << "Adding to command list...\n";
            }
            mClargs.push_back(fragment);
            queryOffset++;
        }
    }
    std::unordered_map<int, std::string> positionals;
    
    for (Uc2::Query query : command.queries()) {
        int position = query.position();
        std::string strQuery = query.format(options);
        if (options & Uc2::VERBOSE) {
            std::cout << "Sorting query: " << strQuery << '\n';
        }
        if (query.queryKind() == QueryKind::Positional) {
            auto clash = positionals.find(position);
            if (clash == positionals.end()) {
                positionals.insert(std::make_pair(position, strQuery));
            } else {
                throw
                    std::string("Clashing positional arguments at index ")
                    + std::to_string(position)
                    + " involving:\n\t"
                    + strQuery
                    + "\n\t"
                    + clash->second;
            }
        } else {
            mClargs.push_back(strQuery);
        }
    }
    // Merge positionals with everything else
    for (auto positional : positionals) {
        int position = positional.first + queryOffset;
        if (position > mClargs.size()) {
            mClargs.insert(mClargs.end(), positional.second);
        } else {
            mClargs.insert(
                mClargs.begin() + position,
                positional.second
            );
        }
    }
}

std::string Uc2::Builder::generate(int options) {
    if (options & Uc2::VERBOSE)
        std::cout << "Generating command...\n";
    std::string command;
    for (auto arg : mClargs) {
        command += arg;
        command += ' ';
    }
    return command;
}
