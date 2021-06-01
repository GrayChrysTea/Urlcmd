#pragma once
#ifndef URLCMD_PARSER_PARSER_HPP
#   define URLCMD_PARSER_PARSER_HPP

#   include <string>
#   include <vector>
#   include <unordered_map>
#   include "constants.hpp"
#   include "utils.hpp"
#   include <urlcmd/typedef.hpp>

namespace Urlcmd::Parser {
    class Command {
        public:
            Command(void) noexcept;
            Command(const std::string _command, Options &_options) noexcept;
            Command(const Command &_other) = default;
            Command(Command &&_other) = default;
            Command &operator=(const Command &_other) = default;
            Command &operator=(Command &&_other) = default;

            std::vector<std::string> result_arr(Options &_options);
            std::string result(Options &_options);
            Command &clear(Options &_options);
            Command &reset(Options &_options);
        private:
            std::string mCommand;
            mutable size_t mIndex;
            mutable std::vector<std::string> mResult;
            URLCMD_OPTION mUrlPart;

            size_t &pNextPart(Options &_options);
            Command &pParse(Options &_options);
    };
}

#endif
