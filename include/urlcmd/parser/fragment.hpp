#pragma once
#ifndef URLCMD_PARSER_FRAGMENT_HPP
#   define URLCMD_PARSER_FRAGMENT_HPP

#   include "baseportion.hpp"

namespace Urlcmd::Parser {
    class Fragment : public BasePortion {
        public:
            virtual int32_t isValid(Options &_options);
            virtual std::string result(Options &_options);
            int32_t done(Options &_options);
            int32_t found(Options &_options);
        private:
        	int32_t mPassed;
            Fragment &pFindEnd(Options &_options);
    };
}

#endif
