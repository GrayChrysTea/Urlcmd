#include <urlcmd/parser.hpp>
#include <iostream>
#include <string>

namespace UcPsr = Urlcmd::Parser;

int32_t UcPsr::Path::isValid(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Path::isValid] "
            << "Checking to see if this command has a valid path segment.\n";
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

std::string UcPsr::Path::result(UcPsr::Options &_options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "[Urlcmd::Parser::Path::result] "
            << "Retrieving the path segment from this command.\n";
    }
    if (this->isValid(_options)) {
        return mStr->substr(mStart, mEnd.value_or(mStart) - mStart);
    } else {
        throw std::string(
            "[Urlcmd::Parser::Path::result] "
            "This command's path segment is invalid."
        );
    }
}

UcPsr::Path &UcPsr::Path::pFindEnd(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "Finding the end character of the path of this command.\n";
    }
    if (mStr == nullptr) {
        throw std::string("[Urlcmd::Parser::Path::findEnd] Null pointer!");
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