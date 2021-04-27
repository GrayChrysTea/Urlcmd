#pragma once
#ifndef URLCMD_PARSER_UTILS_HPP
#   define URLCMD_PARSER_UTILS_HPP

#   include <optional>
#   include "constants.hpp"
#   include <urlcmd/typedef.hpp>

namespace Urlcmd::Parser {
    int32_t isReservedChar(const char _char) noexcept;
    
    inline int32_t hexToInt(const char _hexChar) noexcept;
    
    namespace EscapeCodeErrors {
        static const int32_t BAD_CHAR = -1;
    }
    
    struct EscapeCodeConverter {
        int32_t state;
        char current;
        
        EscapeCodeConverter(const Options &_options) noexcept;
        EscapeCodeConverter &reset(const Options &_options) noexcept;
        int32_t available(const Options &_options) noexcept const;
        int32_t write(const char _hexChar, const Options &_options);
        void hurl(void);
        int32_t isOk(const Options &_options) noexcept const;
        std::optional<char> get(const Options &_options) noexcept;
    };
}

#endif
