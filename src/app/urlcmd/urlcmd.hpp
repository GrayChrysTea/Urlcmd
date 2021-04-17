#pragma once
#ifndef URLCMDAPP_URLCMD_HPP
#   define URLCMDAPP_URLCMD_HPP

#include <vector>
#include <string>
#include <boost/program_options.hpp>

namespace UrlCmd::UrlCmd {
    struct AppArgs {
        int mStatus;

        int mVerbosity;
        int mHelp;
        int mVersion;
        std::vector<std::string> mInput;

        AppArgs(void);
        AppArgs(int, char**);
        AppArgs &reset(void);
        AppArgs &debug(int);
    };

    AppArgs argsWithStatus(int);
    AppArgs processArgs(int, char**);
};

#endif