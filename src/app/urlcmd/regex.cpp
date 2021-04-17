#include "regex.hpp"
#include <iostream>

namespace Uc2 = UrlCmd::UrlCmd;

static const boost::regex re(
    "^(?<path>([-\\w\\d\\^@\\*:%._\\+~=\"'\\\\\\/\\(\\)]+)*)"
    "(?<query>\\?([-\\w\\d\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
        "[-\\w\\d\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+)?"
        "(~([-\\w\\d\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
        "[-\\w\\d\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+))*)?"
    "(?<fragment>#([-\\w\\d\\^@\\*:%._\\+~=\"'\\\\\\/\\(\\)]+)?)?");

static const boost::regex queriesRe(
    "([-\\w\\d\\^@\\*:%._\\+\"'\\\\\\(\\)]+="
    "[-\\w\\d\\^@\\*:%,._\\+\"'\\\\\\/\\(\\)]+)");

static const boost::regex intRe("^\\b\\d+\\b$");

Uc2::Command::Command(
    std::string command,
    int options = 0
) : mCommand(command) {
    destructure(options);
}

int Uc2::Command::destructure(int options = 0) {
    boost::match_results<std::string::const_iterator> match;
    if (!boost::regex_match(mCommand, match, re, boost::match_partial)) {
        if (options | Uc2::VERBOSE) {
            std::cout << "Invalid command detected!\n";
        }
        mStatus = -1;
        return 0;
    }
    if (options | Uc2::VERBOSE)
        std::cout << "\nParsing command...\n"
            << "Path: " << match["path"] << '\n'
            << "Queries: " << match["query"] << '\n'
            << "Fragment: " << match["fragment"] << '\n';
    if (!match["path"].matched) {
        if (options | Uc2::VERBOSE) {
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
        if (options | Uc2::VERBOSE) {
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
            if (options | Uc2::VERBOSE) {
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
            mQueryKind = Uc2::QueryKind::SubCommandOption;
            mRight = rightHand;
        }
        mQueryKind = Uc2::QueryKind::Positional;
        std::string leftIdentifier = leftHand.substr(1);
        if (boost::regex_match(leftIdentifier, match, intRe)) {
            mLeft = leftIdentifier;
            mLeftInt = {std::stoi(leftIdentifier)};
            mRight = rightHand;
        } else if (boost::regex_match(rightHand, match, intRe)) {
            mLeft = leftIdentifier;
            mRight = rightHand;
            mRightInt = {std::stoi(rightHand)};
        } else {
            throw std::string("Value Error! The lvalue or the rvalue must be " "an integer.");
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

std::string Uc2::Query::format(void) {
    if (mQueryKind == Uc2::QueryKind::Option) {
        std::string output("\"-");
        if (mLeft.size() > 1) {
            output += "-";
        }
        output += mLeft;
        output += "\" \"";
        output += mRight;
        output += "\"";
        return output;
    } else if (mQueryKind == Uc2::QueryKind::Flag) {
        std::string output("");
        std::string customLeft("\"-");
        if (mLeft.size() > 1) {
            customLeft += "-";
        }
        customLeft += mLeft;
        customLeft += '\"';
        for (int i = 0; i < mRightInt.value_or(0); i++) {
            output += customLeft;
            output += ' ';
        }
        return output;
    } else if (mQueryKind == Uc2::QueryKind::Positional) {
        
    }
}