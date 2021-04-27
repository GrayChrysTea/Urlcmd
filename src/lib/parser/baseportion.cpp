#include <urlcmd/parser/baseportion.hpp>
#include <iostream>

namespace UcPsr = Urlcmd::Parser;

UcPsr::BasePortion::BasePortion(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 2) {
        std::cout << "Creating a new URL portion parser...\n";
    }
    mStr = _str;
    mStart = _start;
    mIndex = mStart;
}

UcPsr::BasePortion &UcPsr::BasePortion::replaceStr(
    std::string *_str,
    size_t _start,
    UcPsr::Options &_options
) {
    if (_options.verbosity >= 3) {
        std::cout << "Replacing the string being parsed...\n";
    }
    if (_str == NULL) {
        throw std::string("Replacement string cannot be a NULL pointer!");
    }
    mStr = _str;
    mStart = _start;
    mIndex = mStart;
    return *this;
}
