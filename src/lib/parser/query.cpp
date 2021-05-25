#include <urlcmd/parser.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace UcPsr = Urlcmd::Parser;

UcPsr::QueryKindDetector::QueryKindDetector(void) noexcept :
    accumulator("")
{}

UcPsr::QueryKindDetector::QueryKindDetector(
    UcPsr::Options &_options
) noexcept :
    accumulator("")
{
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::QueryKindDetector::QueryKindDetector] "
            << "Creating a new QueryKindDetector.\n";
    }
}

UcPsr::QueryKindDetector &UcPsr::QueryKindDetector::write(
    std::string _input,
    UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::QueryKindDetector::write] "
            << "Writing input into the query kind detector...\n";
    }
    accumulator += _input;
    return *this;
}

URLCMD_OPTION UcPsr::QueryKindDetector::guess(
    UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::QueryKindDetector::guess] "
            << "Guessing what query kind this query is.\n";
    }
    if (UcPsr::hasPrefix(accumulator, UcPsr::QueryKindFlag::POSITIONAL)) {
        return UcPsr::QueryKind::POSITIONAL;
    } else if (UcPsr::hasPrefix(accumulator, UcPsr::QueryKindFlag::FLAG)) {
        return UcPsr::QueryKind::FLAG;
    } else if (
        UcPsr::hasPrefix(accumulator, UcPsr::QueryKindFlag::SUBCOMMANDFLAG)
    ) {
        return UcPsr::QueryKind::SUBCOMMANDFLAG;
    } else if (
        UcPsr::hasPrefix(accumulator, UcPsr::QueryKindFlag::SUBCOMMANDOPTION)
    ) {
        return UcPsr::QueryKind::SUBCOMMANDOPTION;
    } else {
        return UcPsr::QueryKind::OPTION;
    }
}

UcPsr::QueryKindDetector &UcPsr::QueryKindDetector::reset(
    UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::QueryKindDetector::reset] "
            << "Resetting the detector.\n";
    }
    accumulator = "";
    return *this;
}

UcPsr::Query::Query(void) noexcept :
    mKind(0),
    mDetector()
{
    mStr = nullptr;
    mEnd = std::nullopt;
    mEqualsAt = std::nullopt;
    mPosition = std::nullopt;
}

UcPsr::Query::Query(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) noexcept :
    mKind(0),
    mDetector(_options)
{
    if (_options.verbosity >= 2) {
        std::cout
            << "[UcPsr::Query::Query] "
            << "Creating a new query parser...\n";
    }
    mStr = _str;
    mEnd = std::nullopt;
    mEqualsAt = std::nullopt;
    mPosition = std::nullopt;
}

UcPsr::Query::Query(UcPsr::Query &&_other) noexcept :
    mKind(_other.mKind),
    mDetector(std::move(_other.mDetector)),
    mPosition(_other.mPosition)
{
    mStr = _other.mStr;
    _other.mStr = nullptr;
    _other.mStart = 0;
    _other.mEnd = std::nullopt;
    _other.mPosition = 0;
}

UcPsr::Query &UcPsr::Query::operator=(UcPsr::Query &&_other) noexcept {
    mStr = _other.mStr;
    mStart = _other.mStart;
    mEnd = _other.mEnd;
    mKind = _other.mKind;
    mDetector = std::move(_other.mDetector);
    mResult = std::move(_other.mResult);
    mPosition = _other.mPosition;
    _other.mStr = nullptr;
    _other.mStart = 0;
    _other.mEnd = std::nullopt;
    _other.mKind = 0;
    _other.mPosition = 0;
    return *this;
}

UcPsr::Query &UcPsr::Query::replaceStr(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Query::replaceStr] "
            << "Replacing the string being parsed...\n";
    }
    if (_str == nullptr) {
        throw std::string("Replacement string cannot be a NULL pointer!");
    }
    mStr = _str;
    mStart = _start;
    mKind = 0;
    mEnd = std::nullopt;
    return *this;
}

URLCMD_OPTION UcPsr::Query::guessKind(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Query::guessKind] "
            << "Guessing the query type.\n";
    }
    if (!mEqualsAt || !mEnd) {
        throw std::string(
            "Parse this query before running guessKind!"
        );
    }
    mKind = mDetector
        .reset(_options)
        .write(mStr->substr(mStart, mEqualsAt.value() - mStart), _options)
        .guess(_options);
    return mKind;
}

int32_t UcPsr::Query::isValid(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Query::isValid] "
            << "Checking whether this query is valid.\n";
    }
    if (!mEqualsAt || !mEnd) {
        try {
            pFindEnd(_options);
        } catch (...) {
            return 0;
        }
    }
    return mEqualsAt.has_value() && mEnd.has_value();
}

std::string UcPsr::Query::result(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Query::result] "
            << "Checking whether this query is valid.\n";
    }
    if (!this->isValid(_options)) {
        throw std::string("Invalid query.");
    }
    this->guessKind(_options);
    switch (mKind) {
        case UcPsr::QueryKind::POSITIONAL:
            pYieldPositional(_options);
            break;
        case UcPsr::QueryKind::FLAG:
            pYieldFlag(_options);
            break;
        case UcPsr::QueryKind::OPTION:
            pYieldOption(_options);
            break;
        case UcPsr::QueryKind::SUBCOMMANDFLAG:
            pYieldSubcommandFlag(_options);
            break;
        case UcPsr::QueryKind::SUBCOMMANDOPTION:
            pYieldSubcommandOption(_options);
            break;
        default:
            throw std::string("Unknown query kind detected.");
    }
    return mResult;
}

UcPsr::Query &UcPsr::Query::pFindEnd(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Query::pFindEnd] "
            << "Finding the end character of the fragment of this command.\n";
    }
    if (mStr == nullptr) {
        throw std::string(
            "[Urlcmd::Parser::Query::pFindEnd] Null pointer!"
        );
    }
    std::optional<size_t> _result = std::nullopt;
    std::optional<size_t> _equalsAt = std::nullopt;
    size_t _size = mStr->size();
    URLCMD_OPTION _parseState = UcPsr::QueryParseState::LEFT;
    if (!_size) {
        mEnd = 0;
        return *this;
    }
    for (size_t _index = mStart; _index < _size; _index++) {
        char current = mStr->at(_index);
        if (_options.verbosity >= 4) {
            std::cout
                << "\tParsing '"
                << current
                << "' at index "
                << _index
                << '\n';
        }
        if (isReservedChar(current)) {
            if (current == UcPsr::QUERY_SEPARATOR) {
                if (_parseState == UcPsr::QueryParseState::LEFT) {
                    throw std::string(
                        "[Urlcmd::Parser::Query::pFindEnd] "
                        "Premature '"
                    ) + UcPsr::QUERY_SEPARATOR
                    + std::string(
                        "' at index "
                    ) + std::to_string(_index);
                } else if (_parseState == UcPsr::QueryParseState::RIGHT) {
                    _result = _index;
                    break;
                } else {
                    throw std::string(
                        "[Urlcmd::Parser::Query::pFindEnd] "
                        "Invalid Urlcmd::Parser::QueryParseState "
                        "for some reason. "
                        "Diagnosed at index: "
                    ) + std::to_string(_index);
                }
            } else if (current == UcPsr::QUERY_EQUALS) {
                if (_parseState == UcPsr::QueryParseState::LEFT) {
                    _equalsAt = _index;
                    _parseState = UcPsr::QueryParseState::RIGHT;
                    continue;
                } else if (_parseState == UcPsr::QueryParseState::RIGHT) {
                    throw std::string(
                        "[Urlcmd::Parser::Query::pFindEnd] "
                        "Invalid '"
                    ) + UcPsr::QUERY_EQUALS
                    + std::string(
                        "' at index "
                    ) + std::to_string(_index);
                } else {
                    throw std::string(
                        "[Urlcmd::Parser::Query::pFindEnd] "
                        "Invalid Urlcmd::Parser::QueryParseState "
                        "for some reason. "
                        "Diagnosed at index: "
                    ) + std::to_string(_index);
                }
            } else {
                throw std::string(
                    "[Urlcmd::Parser::Query::pFindEnd] "
                    "Unterminated query in command. "
                    "Diagnosed at index: "
                ) + std::to_string(_index);
            }
        }
    }
    mEnd = _result ? _result : _size;
    return *this;
}

std::string UcPsr::Query::pGetLeft(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pGetLeft] "
            << "Getting the left side of the query.\n";
    }
    return mStr->substr(0, mEqualsAt.value());
}

std::string UcPsr::Query::pGetRight(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pGetRight] "
            << "Getting the right side of the query.\n";
    }
    return mStr->substr(mEqualsAt.value() + 1);
}

UcPsr::Query &UcPsr::Query::pYieldPositional(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pYieldPositional] "
            << "Getting positional form of argument.\n";
    }
    std::string _left = pGetLeft(_options).substr(1);
    std::string _right = pGetRight(_options);
    size_t _pos = 0;
    try {
        _pos = boost::lexical_cast<size_t>(_left);
    } catch (const boost::bad_lexical_cast &_e) {
        throw std::string(
            "Invalid left side in this positional query: "
        ) + *mStr;
    }
    mResult = UcPsr::formatPositional(_right, _options);
    mPosition = _pos;
    return *this;
}

UcPsr::Query &UcPsr::Query::pYieldFlag(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pYieldFlag] "
            << "Getting flag form of argument.\n";
    }
    std::string _left = pGetLeft(_options).substr(1);
    std::string _right = pGetRight(_options);
    size_t _reps = 0;
    try {
        _reps = boost::lexical_cast<size_t>(_right);
    } catch (const boost::bad_lexical_cast &_e) {
        throw std::string(
            "Invalid right side in this flag argument: "
        ) + *mStr;
    }
    mResult = UcPsr::formatFlag(_reps, _left, _options);
    return *this;
}

UcPsr::Query &UcPsr::Query::pYieldOption(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pYieldFlag]"
            << "Getting option form of argument.\n";
    }
    mResult = UcPsr::formatOption(
        pGetLeft(_options),
        pGetRight(_options),
        _options
    );
    return *this;
}

UcPsr::Query &UcPsr::Query::pYieldSubcommandFlag(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pYieldSubcommandFlag]"
            << "Getting subcommand flag form of argument.\n";
    }
    mResult = UcPsr::formatOption(
        pGetLeft(_options),
        pGetRight(_options),
        _options
    );
    return *this;
}

UcPsr::Query &UcPsr::Query::pYieldSubcommandOption(UcPsr::Options &_options) {
    if (_options.verbosity >= 4) {
        std::cout
            << "[Urlcmd::Parser::Query::pYieldSubcommandOption]"
            << "Getting subcommand option form of argument.\n";
    }
    std::string _left = pGetLeft(_options).substr(1);
    mResult = UcPsr::formatSubcommandOption(
        _left,
        pGetRight(_options),
        _options
    );
    return *this;
}
