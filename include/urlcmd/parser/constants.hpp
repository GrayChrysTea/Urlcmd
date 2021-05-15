#pragma once
#ifndef URLCMD_PARSER_CONSTANTS_HPP
#   define URLCMD_PARSER_CONSTANTS_HPP

#   include <urlcmd/typedef.hpp>
#   include <string>

namespace Urlcmd::Parser {
    struct Options {
        uint32_t verbosity = 0;
        uint32_t useQuotes = 0;
        URLCMD_OPTION format = 0;
        
        Options(void) noexcept;
        Options(
            uint32_t _verbosity = 0,
            uint32_t _quotes = 0,
            URLCMD_OPTION _format = 0 // ::OutputOptions
        ) noexcept;
        Options(const Options &_other) = default;
        Options(Options &&_other) = default;
        Options &operator=(const Options &_other) = default;
        Options &operator=(Options &&_other) = default;
    };
    
    namespace OutputOptions {
        URLCMD_CONSTOPTION BASH = 0;
        URLCMD_CONSTOPTION DOS = 1;
        URLCMD_CONSTOPTION POWERSHELL = 2;
    }
    
    namespace ParserState {
        URLCMD_CONSTOPTION CHECKING = 0;
        URLCMD_CONSTOPTION CONVERT_HEX = 1;
    }

    namespace UrlPart {
        URLCMD_CONSTOPTION PATH = 1;
        URLCMD_CONSTOPTION QUERY = 2;
        URLCMD_CONSTOPTION FRAGMENT = 3;
    }
    
    namespace QueryKind {
        URLCMD_CONSTOPTION POSITIONAL = 1;       // *1=something
        URLCMD_CONSTOPTION FLAG = 2;             // ^verbose=1
        URLCMD_CONSTOPTION OPTION = 3;           // param=arg
        URLCMD_CONSTOPTION SUBCOMMANDFLAG = 4;   // @@=arg
        URLCMD_CONSTOPTION SUBCOMMANDOPTION = 5; // @subcommand=arg
    }

    namespace QueryKindFlag {
        static const std::string POSITIONAL("*");
        static const std::string FLAG("^");
        static const std::string OPTION("");
        static const std::string SUBCOMMANDFLAG("@@");
        static const std::string SUBCOMMANDOPTION("@");
    }
    
    namespace QueryParseState {
        URLCMD_CONSTOPTION LEFT = 1;
        URLCMD_CONSTOPTION RIGHT = 2;
    }
    
    static const char QUERY_START = '?';
    static const char QUERY_SEPARATOR = '&';
    static const char QUERY_EQUALS = '=';
    static const char FRAGMENT_START = '#';
    static const char RESERVED_CHARS[] = {
        QUERY_START,
        QUERY_SEPARATOR,
        QUERY_EQUALS,
        FRAGMENT_START
    };
    static const size_t RESERVED_CHARS_SIZE = (
        sizeof(RESERVED_CHARS) / sizeof(char)
    );
}

#endif
