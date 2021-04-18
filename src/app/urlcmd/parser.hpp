#pragma once
#ifndef URLCMDAPP_REGEX_HPP
#   define URLCMDAPP_REGEX_HPP

#   include <boost/regex.hpp>
#   include <string>
#   include <vector>
#   include <optional>

namespace UrlCmd::UrlCmd {
    static int DEFAULT = 0;
    static int VERBOSE = 1;
    static int DOSFORM = 2;

    enum class QueryKind : int {
        Positional = 1, // @1=something
        Flag = 2, //^verbose=1
        Option = 3, //param=arg
        SubCommandFlag = 4, //@@=arg
        SubCommandOption = 5 //@subcommand=arg
    };

    class Query {
        public:
            Query(std::string query);
            QueryKind queryKind(void);
            std::string left(void);
            std::string right(void);
            std::string format(int options = 0);
            int position(void);
    
        private:
            QueryKind mQueryKind;
            std::string mLeft;
            std::string mRight;
            std::optional<int> mLeftInt;
            std::optional<int> mRightInt;
    };
    
    class Command {
        public:
            Command(std::string command, int options = 0);
            int processed(void);
            int ok(void);
            int err(void);
            std::string path(void);
            std::vector<Query> queries(void);
            std::string fragment(void);
        private:
            int destructure(int options = 0);
            Command &clear(void);

            int mStatus;
            std::string mCommand;
            std::string mPath;
            std::string mQueryUnsplit;
            std::vector<std::string> mQueries;
            std::string mFragment;
    };


    class Builder {
        public:
            Builder(Command& command, int options = 0);
            std::string generate(int options = 0);
        private:
            std::vector<std::string> mClargs;
    };
};

#endif
