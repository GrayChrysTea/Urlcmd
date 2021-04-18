#pragma once
#ifndef URLCMDAPP_URLCMD_HPP
#   define URLCMDAPP_URLCMD_HPP

#include <vector>
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace UrlCmd::UrlCmd {
    int printHelp(po::options_description &visible, int options = 0);
    int printGeneral(int options = 0);

    struct AppArgs {
        int mStatus;

        int mVerbosity;
        int mHelp;
        int mVersion;
        int mPreview;
        int mDosForm;
        int mSkip;
        std::vector<std::string> mInput;

        AppArgs(void);
        AppArgs(int argc, char **argv);
        AppArgs &reset(void);
        AppArgs &debug(int flags = 0);
    };

    AppArgs argsWithStatus(int status = 0);
    AppArgs processArgs(int argc, char **argv);
};

#endif
