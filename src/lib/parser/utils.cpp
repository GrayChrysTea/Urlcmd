#include <iostream>
#include <urlcmd/parser/constants.hpp>
#include <urlcmd/parser/parser.hpp>

namespace UcPsr = Urlcmd::Parser;

int32_t UcPsr::isReservedChar(const char _char) noexcept {
    for (size_t _index = 0; _index < RESERVED_CHARS_SIZE; _index++) {
        if (_char == RESERVED_CHARS[_index]) {
            return 1;
        }
    }
    return 0;
}

inline int32_t UcPsr::hexToInt(const char _hexChar) noexcept {
    if ('0' <= _hexChar && _hexChar <= '9') {
        return _hexChar - '0';
    } else if ('A' <= _hexChar && _hexChar <= 'F') {
        return _hexChar - 'A' + 10;
    } else if ('a' <= _hexChar && _hexChar <= 'f') {
        return _hexChar - 'a' + 10;
    } else {
        return -1;
    }
}

int32_t UcPsr::hasPrefix(
    const std::string &_main,
    const std::string &_prefix
) noexcept {
    return _main.substr(0, _prefix.size()) == _prefix;
}

UcPsr::EscapeCodeConverter::EscapeCodeConverter(void) noexcept :
    state(0),
    current(0)
{}

UcPsr::EscapeCodeConverter::EscapeCodeConverter(
    const UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 3) {
        std::cout << "Creating a new escape code converter object...\n";
    }
}

UcPsr::EscapeCodeConverter &UcPsr::EscapeCodeConverter::reset(
    const UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 3) {
        std::cout
            << "Resetting this escape code converter at "
            << this
            << "...\n";
    }
    state = 0;
    current = 0;
    return *this;
}

int32_t UcPsr::EscapeCodeConverter::available(
    const UcPsr::Options &_options
) const noexcept {
    if (_options.verbosity >= 3) {
        std::cout
            << "Checking to see if this escape code converter "
               "can accept more hexadecimal characters...\n";
    }
    return state == 0 || state == 1;
}

int32_t UcPsr::EscapeCodeConverter::write(
    const char _hexChar,
    const UcPsr::Options &_options
) noexcept {
    if (_options.verbosity >= 3) {
        std::cout
            << "Writing "
            << _hexChar
            << " to escape code converter at "
            << this
            << "...\n";
    }
    if (!this->available(_options)) {
        return 1;
    }
    int32_t _integer = UcPsr::hexToInt(_hexChar);
    if (_integer < 0) {
        state = UcPsr::EscapeCodeErrors::BAD_CHAR;
    } else {
        current += _integer;
        if (state == 0) {
            current <<= 4;
        }
        state += 1;
    }
    return 0;
}

void UcPsr::EscapeCodeConverter::hurl(const Options &_options) const {
    if (this->available(_options)) {
        throw std::string("Incomplete parsing of escape code!");
    }
}

int32_t UcPsr::EscapeCodeConverter::isOk(
    const UcPsr::Options &_options
) const noexcept {
    if (_options.verbosity >= 3) {
        std::cout
            << "Checking to see if this escape code has been parsed "
            << "correctly.\n";
    }
    return this->available(_options);
}

std::optional<char> UcPsr::EscapeCodeConverter::get(
    const UcPsr::Options &_options
) const {
    if (_options.verbosity >= 3) {
        std::cout
            << "Attempting to grab the character form by an escape code.\n";
    }
    if (this->available(_options)) {
        return std::nullopt;
    }
    return current;
}
