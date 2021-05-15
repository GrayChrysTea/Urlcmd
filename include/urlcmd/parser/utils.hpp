#pragma once
#ifndef URLCMD_PARSER_UTILS_HPP
#   define URLCMD_PARSER_UTILS_HPP

#   include <optional>
#   include <string>
#   include "constants.hpp"
#   include <urlcmd/typedef.hpp>

namespace Urlcmd::Parser {
    int32_t isReservedChar(const char _char) noexcept;
    
    inline int32_t hexToInt(const char _hexChar) noexcept;

    int32_t hasPrefix(
        const std::string &_main,
        const std::string &_prefix
    ) noexcept;
    
    namespace EscapeCodeErrors {
        static const int32_t BAD_CHAR = -1;
    }
    
    struct EscapeCodeConverter {
        int32_t state;
        char current;
        
        EscapeCodeConverter(void) noexcept;
        EscapeCodeConverter(const Options &_options) noexcept;
        EscapeCodeConverter(const EscapeCodeConverter &_other) = default;
        EscapeCodeConverter(EscapeCodeConverter &&_other) = default;
        EscapeCodeConverter &operator=(
            const EscapeCodeConverter &_other
        ) = default;
        EscapeCodeConverter &operator=(EscapeCodeConverter &&_other) = default;
        EscapeCodeConverter &reset(const Options &_options) noexcept;
        int32_t available(const Options &_options) const noexcept;
        int32_t write(const char _hexChar, const Options &_options) noexcept;
        void hurl(const Options &_options) const;
        int32_t isOk(const Options &_options) const noexcept;
        std::optional<char> get(const Options &_options) const;
    };
}

#endif
