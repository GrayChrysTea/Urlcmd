#pragma once
#ifndef URLCMD_PARSER_PATH_HPP
#   define URLCMD_PARSER_PATH_HPP

#   include "baseportion.hpp"

namespace Urlcmd::Parser {
    class Path : public BasePortion {
        public:
            virtual int32_t isValid(Options &_options);
            virtual std::string result(Options &_options);
        private:
            Path &pFindEnd(Options &_options);
    };
}

#endif