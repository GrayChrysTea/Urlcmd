#pragma once
#ifndef URLCMD_PARSER_QUERYFORMAT_HPP
#   define URLCMD_PARSER_QUERYFORMAT_HPP

#   include "baseportion.hpp"
#   include "constants.hpp"
#   include <string>

namespace Urlcmd::Parser {
    std::string formatPositional(
        const std::string &_right,
        Options &_options
    );

    std::string formatFlag(
        size_t _reps,
        const std::string &_left,
        Options &_options
    );

    std::string formatOption(
        const std::string &_left,
        const std::string &_right,
        Options &_options
    );

    std::string formatSubcommandFlag(
        const std::string &_right,
        Options &_options
    );

    std::string formatSubcommandOption(
        const std::string &_left,
        const std::string &_right,
        Options &_options
    );
}

#endif