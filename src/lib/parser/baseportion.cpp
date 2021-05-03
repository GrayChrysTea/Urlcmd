#include <urlcmd/parser/baseportion.hpp>
#include <iostream>

namespace UcPsr = Urlcmd::Parser;

UcPsr::BasePortion::BasePortion(void) :
    mStart(0),
    mEnd(0)
{
    mStr = nullptr;
}

UcPsr::BasePortion::BasePortion(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) {
    if (_options.verbosity >= 2) {
        std::cout << "Creating a new URL portion parser...\n";
    }
    mStr = _str;
    mStart = _start;
    mEnd = std::nullopt;
}

UcPsr::BasePortion::BasePortion(UcPsr::BasePortion &&_other) :
    mStart(_other.mStart),
    mEnd(_other.mEnd),
    mResult(std::move(_other.mResult))
{
    mStr = _other.mStr;
    _other.mStr = nullptr;
    _other.mStart = 0;
    _other.mEnd = 0;
}

UcPsr::BasePortion &UcPsr::BasePortion::operator=(
    UcPsr::BasePortion &&_other
) {
    mStr = _other.mStr;
    mStart = _other.mStart;
    mEnd = _other.mEnd;
    mResult = std::move(_other.mResult);
    _other.mStr = nullptr;
    _other.mStart = 0;
    _other.mEnd = 0;
    return *this;
}

UcPsr::BasePortion &UcPsr::BasePortion::replaceStr(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) {
    if (_options.verbosity >= 3) {
        std::cout << "Replacing the string being parsed...\n";
    }
    if (_str == nullptr) {
        throw std::string("Replacement string cannot be a NULL pointer!");
    }
    mStr = _str;
    mStart = _start;
    mEnd = std::nullopt;
    return *this;
}

