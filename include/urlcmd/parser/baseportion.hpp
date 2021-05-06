#pragma once
#ifndef URLCMD_PARSER_BASEPORTION_HPP
#   define URLCMD_PARSER_BASEPORTION_HPP

#   include <cstdlib>
#   include <string>
#   include <optional>
#   include "constants.hpp"

namespace Urlcmd::Parser {
    // Abstract base class for path, query and fragment parsers
    class BasePortion {
        public:
            BasePortion(void) noexcept;
            BasePortion(
                std::string *_str,
                size_t _start,
                Options &_options
            ) noexcept;
            // BasePortion(const BasePortion &_other) = default;
            BasePortion(BasePortion &&_other) noexcept;
            // BasePortion &operator=(const BasePortion &_other) = default;
            BasePortion &operator=(BasePortion &&_other) noexcept;
            
            BasePortion &replaceStr(
                std::string *_str,
                size_t _start,
                Options &_options
            );
            virtual int32_t isValid(Options &_options) = 0;
            virtual std::string result(Options &_options) = 0;
        protected:
            std::string *mStr;
            size_t mStart;
            mutable std::optional<size_t> mEnd;
            mutable std::string mResult;
    };
}

#endif
