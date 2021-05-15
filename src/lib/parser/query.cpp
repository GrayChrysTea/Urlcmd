#include <urlcmd/parser/query.hpp>
#include <urlcmd/parser/utils.hpp>
#include <iostream>

namespace UcPsr = Urlcmd::Parser;

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

URLCMD_CONSTOPTION UcPsr::QueryKindDetector::guess(
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

UcPsr::Query &replaceStr(
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

int32_t UcPsr::Query::isValid(UcPsr::Options &_options) {

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
    URLCMD_CONSTOPTION _parseState = UcPsr::QueryParseState::LEFT;
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
                    ) + std::to_string(index);
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
                    ) + std::to_string(index);
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