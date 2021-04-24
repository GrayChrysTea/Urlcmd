#pragma once
#ifndef URLCMD_PARSER_PARSER_HPP
#   define URLCMD_PARSER_PARSER_HPP

#   include <string>
#   include <vector>
#   include <unordered_map>
#   include "constants.hpp"
#   include <urlcmd/typedef.hpp>



namespace Urlcmd::Parser {
    static const char QUERY_START = '?';
    static const char QUERY_SEPARATOR = '&';
    static const char FRAGMENT_START = '#';
    static const char RESERVED_CHARS[3] = {
        QUERY_START,
        QUERY_SEPARATOR,
        FRAGMENT_START
    };
    static const size_t RESERVED_CHARS_SIZE = (
        sizeof(RESERVED_CHARS) / sizeof(char)
    );
    
    int32_t isReservedChar(char _char);
    
    inline int32_t hexToInt(char _hexChar);
    
    namespace EscapeCodeErrors {
        static const int32_t BAD_CHAR = -1;
    }
    
    struct EscapeCodeConverter {
        int32_t state;
        char current;
        
        EscapeCodeConverter(Options _options);
        EscapeCodeConverter &reset(Options _options);
        int32_t available(Options _options);
        int32_t write(char _hexChar, Options _options);
    };
    
    class Command {
        public:
            Command(Options _options);
            Command(std::string _command, Options _options);
            Command &setCommand(std::string _command, Options _options);
            Command &clear(Options _options);
            int32_t isDone(Options _options);
            int32_t hasError(Options _options);
            std::string getError(Options _options);
            Command &discardError(Options _options);
            Command &process(Options _options);
            
        private:
            int32_t pCheck(Options _options);
            int32_t pWrite(Options _options);
            int32_t pProcessNext(Options _options);

            size_t mIndex;
            URLCMD_OPTION mUrlPart;
            URLCMD_OPTION mState;
            URLCMD_OPTION mQueryParseState;
            URLCMD_OPTION mQueryKind;
            EscapeCodeConverter mEscConverter;
            std::string mCommand;
            std::string mPath;
            std::string mFragment;
            std::vector<std::string> mArgs;
            std::unordered_map<size_t, std::string> mPositionals;
            std::string mErr;
    };
}

#endif
