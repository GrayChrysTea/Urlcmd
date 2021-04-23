#pragma once
#ifndef URLCMD_PARSER_PARSER_HPP
#   define URLCMD_PARSER_PARSER_HPP

#   include <string>
#   include <vector>
#   include "constants.hpp"

namespace Urlcmd::Parser {
    class Command {
        public:
            Command(std::string command, int32 options = DEFAULT);
            int32 clear(void);
        private:
            int32 pCheck(int32 options = DEFAULT);
            int32 pWrite(int32 options = DEFAULT);
            
            std::string mCommand;
            std::vector<std::string> mArgs;
    };
}

#endif
