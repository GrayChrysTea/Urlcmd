#include <urlcmd/parser/parser.hpp>
#include <iostream>

namespace UcPsr = Urlcmd::Parser;

int32_t UcPsr::isReservedChar(char _char) {
    for (size_t _index = 0; _index < RESERVED_CHARS_SIZE; _index++) {
        if (_char == RESERVED_CHARS[_index]) {
            return 1;
        }
    }
    return 0;
}

inline int32_t UcPsr::hexToInt(char _hexChar) {
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

UcPsr::EscapeCodeConverter::EscapeCodeConverter(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout << "Creating a new escape code converter object...\n";
    }
}

UcPsr::EscapeCodeConverter &UcPsr::EscapeCodeConverter::reset(
    UcPsr::Options _options
) {
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

int32_t UcPsr::EscapeCodeConverter::available(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "Checking to see if this escape code converter "
               "can accept more hexadecimal characters...\n";
    }
    return state == 0 || state == 1;
}

int32_t UcPsr::EscapeCodeConverter::write(
    char _hexChar,
    UcPsr::Options _options
) {
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

UcPsr::Command::Command(UcPsr::Options _options) : mEscConverter(_options) {}

UcPsr::Command::Command(
    std::string _command,
    UcPsr::Options _options
) :
    mEscConverter(_options)
{
    mCommand = _command;
    if (_options.verbosity >= 2) {
        std::cout
            << "Creating command parser for the following command:\n\t"
            << mCommand
            << '\n';
    }
}

UcPsr::Command &UcPsr::Command::setCommand(
    std::string _command,
    UcPsr::Options _options
) {
    if (_options.verbosity >= 2) {
        std::cout << "Setting a new command for this parser...\n";
    }
    this->clear(_options);
    mCommand = _command;
    if (_options.verbosity >= 2) {
        std::cout << "New command: " << mCommand << '\n';
    }
    return *this;
}

UcPsr::Command &UcPsr::Command::clear(UcPsr::Options _options) {
    if (_options.verbosity >= 2) {
        std::cout
            << "Clearing command parser at "
            << this
            << "...\n";
    }
    mIndex = 0;
    mUrlPart = URLCMD_OPTIONDEFAULT;
    mState = URLCMD_OPTIONDEFAULT;
    mQueryParseState = URLCMD_OPTIONDEFAULT;
    mQueryKind = URLCMD_OPTIONDEFAULT;
    mEscConverter.reset(_options);
    mPath.clear();
    mFragment.clear();
    mArgs.clear();
    mPositionals.clear();
    mErr.clear();
    return *this;
}

int32_t UcPsr::Command::isDone(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "Checking to see if this parser has "
               "finished parsing this command.\n";
    }
    return mIndex >= mCommand.size();
}

int32_t UcPsr::Command::hasError(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "Checking to see if this parser has noticed an error.\n";
    }
    return mErr.size() > 0;
}

std::string UcPsr::Command::getError(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout << "Getting error message...\n";
    }
    return mErr;
}

UcPsr::Command &UcPsr::Command::discardError(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout << "Discarding error message...\n";
    }
    mErr.clear();
    return *this;
}

int32_t UcPsr::Command::pProcessNext(UcPsr::Options _options) {
    if (_options.verbosity >= 3) {
        std::cout << "Processing next character in command.\n";
    }
    if (this->isDone(_options)) {
        return 1;
    }
    if (mUrlPart == URLCMD_OPTIONDEFAULT) {
        mUrlPart = UcPsr::UrlPart::PATH;
    }
    if (UcPsr::UrlPart::PATH) {
        
    }
    return 0;
}
