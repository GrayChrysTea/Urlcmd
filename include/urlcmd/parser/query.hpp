#pragma once
#ifndef URLCMD_PARSER_QUERY_HPP
#   define URLCMD_PARSER_QUERY_HPP

#   include "baseportion.hpp"
#   include "constants.hpp"
#   include <string>

namespace Urlcmd::Parser {
    struct QueryKindDetector {
        std::string accumulator;

        QueryKindDetector(void) noexcept;
        QueryKindDetector(Options &_options) noexcept;
        QueryKindDetector(const QueryKindDetector &_other) = default;
        QueryKindDetector(QueryKindDetector &&_other) = default;
        QueryKindDetector &operator=(const QueryKindDetector &_other) = default;
        QueryKindDetector &operator=(QueryKindDetector &&_other) = default;

        QueryKindDetector &write(
            std::string _input,
            Options &_options
        ) noexcept;
        URLCMD_OPTION guess(Options &_options) noexcept;
        QueryKindDetector &reset(Options &_options) noexcept;
    }

    class Query : public Query {
        public:
            Query(void) noexcept override;
            Query(
                std::string *_str,
                size_t _start,
                Options &_options
            ) noexcept override;
            Query(Query &&_other) noexcept override;
            Query &operator=(Query &&_other) noexcept override;

            Query &replaceStr(
                std::string *_str,
                size_t _start,
                Options &_options
            ) override;

            URLCMD_OPTION guessKind(Options &_options);
            virtual int32_t isValid(Options &_options);
            virtual std::string result(Options &_options);
            std::optional<size_t> position(Options &_options);
        private:
            mutable QueryKindDetector mDetector;
            mutable URLCMD_OPTION mKind;
            mutable std::optional<size_t> mEqualsAt;
            mutable std::optional<size_t> mPosition;
            Query &pFindEnd(Options &_options);
            std::string pGetLeft(Options &_options);
            std::string pGetRight(Options &_options);
            Query &pYieldPositional(Options &_options);
            Query &pYieldFlag(Options &_options);
            Query &pYieldOption(Options &_options);
            Query &pYieldSubcommandFlag(Options &_options);
            Query &pYieldSubcommandOption(Options &_options);
    }
}

#endif