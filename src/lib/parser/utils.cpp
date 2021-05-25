#include <cctype>
#include <iostream>
#include <string>
#include <urlcmd/error.hpp>
#include <urlcmd/parser/utils.hpp>
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

int32_t hasWhitespace(const std::string &_str) noexcept {
    for (auto _it : _str) {
        if (std::isspace(_it)) {
            return 1;
        }
    }
    return 0;
}

std::string UcPsr::convertEscapeCodes(
    const std::string &_str,
    UcPsr::Options &_options
) {
    UcPsr::EscapeCodeConverter _converter(_options);
    std::string _result = "";
    for (size_t _index = 0; _index < _str.size(); _index++) {
        char _current = _str[_index];
        int32_t _state = _converter.write(_current, _options);
        if (URLCMD_ISWARN(_state)) {
            _result += _converter.get(_options).value();
            _converter.reset(_options);
            _state = _converter.write(_current, _options);
            if (!URLCMD_ISOK(_state)) {
                throw std::string("Could not decode ")
                    + _current
                    + " at index " 
                    + std::to_string(_index);
            }
        }
    }
    return _result;
}

std::string UcPsr::generateFlag(
    const std::string &_str,
    UcPsr::Options &_options
) {
    std::string _converted = UcPsr::convertEscapeCodes(_str, _options);
    if (_converted.size() < 1) {
        if (_options.format == UcPsr::OutputOptions::DOS) {
            throw std::string("Flag cannot be empty.");
        } else {
            return std::string("--");
        }
    } else if (_converted.size() == 1) {
        if (_options.format == UcPsr::OutputOptions::DOS) {
            _converted.insert(0, "/");
            return UcPsr::smartDelimit(_converted, _options);
        } else {
            _converted.insert(0, "-");
            return UcPsr::delimit(_converted);
        }
    } else {
        if (_options.format == UcPsr::OutputOptions::DOS) {
            _converted.insert(0, "/");
            return UcPsr::smartDelimit(_converted, _options);
        } else {
            _converted.insert(0, "--");
            return UcPsr::delimit(_converted);
        }
    }
}

std::string UcPsr::delimit(const std::string &_str) noexcept {
    std::string _result = _str;
    _result.insert(0, "\"");
    _result.insert(_str.size(), "\"");
    return _result;
}

std::string UcPsr::smartDelimit(
    const std::string &_str,
    UcPsr::Options &_options
) noexcept {
    switch (_options.format) {
        case UcPsr::OutputOptions::DOS:
            if (UcPsr::hasWhitespace(_str)) {
                return UcPsr::delimit(_str);
            } else {
                return _str;
            }
        default:
            return UcPsr::delimit(_str);
    }
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
    return state == 1 || state == 2;
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
    if (state == 0) {
        if (_hexChar == UcPsr::ESCCODE_START) {
            state = 1;
            return 0;
        } else {
            // If we are not already parsing an escape code,
            // ignore the character altogether
            return 0;
        }
    }
    if (!this->isOk(_options)) {
        return 1;
    }
    if (!this->available(_options)) {
        return -1;
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

int32_t UcPsr::EscapeCodeConverter::write_str(
    const std::string &_str,
    const Options &_options
) noexcept {
    if (_options.verbosity >= 3) {
        std::cout
            << "Writing string to EscapeCodeConverter.\n";
    }
    for (auto _it : _str) {
        if (!this->isOk(_options)) {
            return 1;
        }
        if (!this->available(_options)) {
            return 0;
        }
        int32_t _status = this->write(_it, _options);
        if (!URLCMD_ISOK(_status)) {
            return _status;
        }
    }
    return 1;
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
    return this->available(_options) || state == 0;
}

std::optional<std::string> UcPsr::EscapeCodeConverter::get(
    const UcPsr::Options &_options
) const {
    if (_options.verbosity >= 3) {
        std::cout
            << "Attempting to grab the character formed by an escape code.\n";
    }
    if (this->available(_options)) {
        return std::nullopt;
    }
    if (current == '"') {
        return std::string("\"");
    } else {
        return std::string(1, current);
    }
}
