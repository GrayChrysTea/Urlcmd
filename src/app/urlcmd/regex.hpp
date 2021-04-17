#pragma once
#ifndef URLCMDAPP_REGEX_HPP
#   define URLCMDAPP_REGEX_HPP

#   include <boost/regex.hpp>
#   include <string>
#   include <vector>
#   include <optional>

namespace UrlCmd::UrlCmd {
    static int DEFAULT = 1;
    static int VERBOSE = 1;

    class Command {
        public:
            Command(std::string, int);
            int processed(void);
            int ok(void);
            int err(void);
            std::string path(void);
            std::vector<std::string> queries(void);
            std::string fragment(void);
        private:
            int destructure(int);
            Command &clear(void);

            int mStatus;
            std::string mCommand;
            std::string mPath;
            std::string mQueryUnsplit;
            std::vector<std::string> mQueries;
            std::string mFragment;
    };

    enum class QueryKind : int {
        Positional = 1, // @1=something
        Flag = 2, //^verbose=1
        Option = 3, //param=arg
        SubCommandOption = 4 //@@=arg
    };

    class Query {
        public:
            Query(std::string);
            QueryKind queryKind(void);
            std::string left(void);
            std::string right(void);
            std::string format(void);
    
        private:
            QueryKind mQueryKind;
            std::string mLeft;
            std::string mRight;
            std::optional<int> mLeftInt;
            std::optional<int> mRightInt;
    };
};

#endif