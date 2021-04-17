#include "urlcmd.hpp"
#include <iostream>
#include <urlcmd/urlcmd.h>
#include <urlcmd/utils.hpp>
#include <boost/program_options.hpp>

namespace Uc2 = UrlCmd::UrlCmd;
namespace Ut = UrlCmd::Utils;
namespace po = boost::program_options;

Uc2::AppArgs::AppArgs(void) : mInput() {
    mStatus = 0;
    mVersion = 0;
    mHelp = 0;
    mVerbosity = 0;
}

Uc2::AppArgs::AppArgs(int argc, char **argv) {
    mStatus = 0;

    po::options_description flags("Flags");
    flags.add_options()
        ("help,h", "Display this help message.")
        ("version,V", "Display the version of this urlcmd.");
    po::options_description options("Options");
    options.add_options()
        (
            "verbose,v",
            po::value<int>(&this->mVerbosity)->default_value(0),
            "Set the verbosity for urlcmd."
        );
    po::options_description hidden("Hidden Options");
    hidden.add_options()
        (
            "input-commands,c",
            po::value<std::vector<std::string>>(&this->mInput),
            "The commands you want to input to urlcmd."
        );
    
    po::options_description cmdline_options;
    cmdline_options.add(flags).add(options).add(hidden);
    po::options_description visible_options(URLCMD_FULLNAME);
    visible_options.add(flags).add(options);
    
    po::positional_options_description positionals;
    positionals.add("input-commands", -1);

    po::variables_map vmap;
    po::store(
        po::command_line_parser(argc, argv)
            .options(cmdline_options)
            .positional(positionals)
            .run(),
        vmap
    );
    po::notify(vmap);

    mHelp = vmap.count("help");
    mVersion = vmap.count("version");

    if (mHelp) {
        std::cout << visible_options;
    } else if (mVersion) {
        std::cout << URLCMD_FULLNAME << '\n';
        std::cout << "\nWritten by <!-- Renoir Tan -->\n";
    }
}

Uc2::AppArgs &Uc2::AppArgs::reset(void) {
    mVerbosity = 0;
    mHelp = 0;
    mVersion = 0;
    mInput.clear();
    return *this;
}

Uc2::AppArgs &Uc2::AppArgs::debug(int flags = 0) {
    std::cout << URLCMD_FULLNAME " AppArgs\n\n"
        << "Status: " << mStatus << '\n'
        << "Verbosity: " << mVerbosity << '\n'
        << "Version: " << mVersion << '\n'
        << "Help: " << mHelp << '\n'
        << "Commands:\n" 
        << Ut::vectorToString(&mInput, 1, 1);
    return *this;
}

Uc2::AppArgs Uc2::argsWithStatus(int status = 0) {
    Uc2::AppArgs options;
    options.mStatus = status;
    return options;
}

Uc2::AppArgs Uc2::processArgs(int argc, char **argv) {
    try {
        Uc2::AppArgs options(argc, argv);
        if (options.mHelp || options.mVersion) {
            options.mStatus = 1;
        }
        return options;
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
        return Uc2::argsWithStatus(-1);
    }
}
