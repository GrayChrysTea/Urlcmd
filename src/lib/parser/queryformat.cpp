#include <urlcmd/parser.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

namespace UcPsr = Urlcmd::Parser;

std::string UcPsr::formatPositional(
    const std::string &_right,
    UcPsr::Options &_options
) {
    return UcPsr::convertEscapeCodes(_right, _options);
}

std::string UcPsr::formatFlag(
    size_t _reps,
    const std::string &_left,
    UcPsr::Options &_options
) {
    std::string _flag = UcPsr::generateFlag(
        UcPsr::convertEscapeCodes(_left, _options),
        _options
    );
    std::string _result;
    for (size_t _index; _index < _reps; _index++) {
        _result += _flag + " ";
    }
    return _result;
}

std::string UcPsr::formatOption(
    const std::string &_left,
    const std::string &_right,
    UcPsr::Options &_options
) {
    std::string _flag = UcPsr::generateFlag(
        UcPsr::convertEscapeCodes(_left, _options),
        _options
    );
    std::string _arg = UcPsr::smartDelimit(
        UcPsr::convertEscapeCodes(_right, _options),
        _options
    );
    if (_options.format == UcPsr::OutputOptions::DOS) {
        return _flag + ":" + _arg;
    } else {
        return _flag + " " + _arg;
    }
}

std::string UcPsr::formatSubcommandFlag(
    const std::string &_right,
    UcPsr::Options &_options
) {
    return UcPsr::smartDelimit(_right, _options);
}

std::string UcPsr::formatSubcommandOption(
    const std::string &_left,
    const std::string &_right,
    UcPsr::Options &_options
) {
    return
        UcPsr::smartDelimit(_left, _options)
        + " "
        + UcPsr::smartDelimit(_right, _options);
}