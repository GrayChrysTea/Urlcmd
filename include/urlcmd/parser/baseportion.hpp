#pragma once
#ifndef URLCMD_PARSER_BASEPORTION_HPP
#   define URLCMD_PARSER_BASEPORTION_HPP

#   include <cstdlib>
#   include <string>
#   include "constants.hpp"

namespace Urlcmd::Parser {
    class BasePortion {
        public:
            BasePortion(
                std::string *_str, size_t _start, Options &_options) noexcept;
            BasePortion &replaceStr(
                std::string *_str, size_t _start, Options &_options);
        private:
            std::string *mStr;
            size_t mStart;
            size_t mIndex;
    }
}

#endif
