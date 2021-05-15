#pragma once
#ifndef URLCMD_PARSER_QUERY_HPP
#   define URLCMD_PARSER_QUERY_HPP

#   include "baseportion.hpp"
#   include "constants.hpp"
#   include <string>

namespace Urlcmd::Parser {
    struct QueryKindDetector {
        std::string accumulator;

        QueryKindDetector(Options &_options) noexcept;
        QueryKindDetector &write(
            std::string _input,
            Options &_options
        ) noexcept;
        URLCMD_CONSTOPTION guess(Options &_options) noexcept;
        QueryKindDetector &reset(Options &_options) noexcept;
    }

    class Query : public Query {
        public:
            Query &replaceStr(
                std::string *_str,
                size_t _start,
                Options &_options
            ) override;

            virtual int32_t isValid(Options &_options);
            virtual std::string result(Options &_options);
        private:
            URLCMD_CONSTOPTION mKind;
            std::optional<size_t> mEqualsAt;
            Query &pFindEnd(Options &_options);
    }
}

#endif