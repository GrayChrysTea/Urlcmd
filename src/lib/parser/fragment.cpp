#include <urlcmd/parser.hpp>
#include <iostream>
#include <string>

namespace UcPsr = Urlcmd::Parser;

int32_t UcPsr::Fragment::isValid(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Fragment::isValid] "
            << "Checking to see if this command has a valid fragment.\n";
    }
    if (mStr == nullptr) {
        return 0;
    }
    try {
        if (!mEnd) {
            this->pFindEnd(_options);
        }
    } catch (std::string &_e) {
        return 0;
    }
    return mEnd.has_value();
}

std::string UcPsr::Fragment::result(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Fragment::result] "
            << "Retrieving the fragment from this command.\n";
    }
    if (this->isValid(_options)) {
        return mStr->substr(mStart, mEnd.value_or(mStart) - mStart);
    } else {
        throw std::string(
            "[Urlcmd::Parser::Fragment::result] "
            "This command's fragment is invalid."
        );
    }
}

int32_t UcPsr::Fragment::done(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Fragment::result] "
            << "Checking to see if this parser has been run.\n";
    }
    return mPassed;
}

int32_t UcPsr::Fragment::found(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Fragment::result] "
            << "Checking to see if a fragment has been found.\n";
    }
    return mEnd.has_value();
}

UcPsr::Fragment &UcPsr::Fragment::pFindEnd(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "Finding the end character of the fragment of this command.\n";
    }
    if (mStr == nullptr) {
        throw std::string(
            "[Urlcmd::Parser::Fragment::pFindEnd] Null pointer!"
        );
    }
    std::optional<size_t> _result = std::nullopt;
    size_t _size = mStr->size();
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
            if (current == UcPsr::QUERY_START) {
                _result = _index;
                break;
            } else {
                throw std::string(
                    "[Urlcmd::Parser::Path::findEnd] "
                    "Unterminated path segment in command. "
                    "Diagnosed at index: "
                ) + std::to_string(_index);
            }
        }
    }
    mEnd = _result ? _result : _size;
    return *this;
}