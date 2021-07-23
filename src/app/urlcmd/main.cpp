#include <urlcmd/urlcmd.h>
#include <urlcmd/error.hpp>
#include <iostream>
#include <cstdlib>
#include <boost/program_options.hpp>
#include "appargs.hpp"
#include "parser.hpp"

namespace po = boost::program_options;
namespace Uc2 = UrlCmd::UrlCmd;

int run(int argc, char *argv[]) {
    Uc2::AppArgs options = Uc2::processArgs(argc, argv);
    if (options.mVerbosity)
        options.debug(0);
    if (options.mStatus > 0)
        return 0;
    else if (options.mStatus < 0) {
        std::cout << "Could not parse command line arguments... :(\n";
        return 1;
    }
    for (auto input : options.mInput) {
        int parseOptions = Uc2::DEFAULT;
        if (options.mVerbosity) {
            parseOptions |= Uc2::VERBOSE;
        }
        if (options.mDosForm) {
            parseOptions |= Uc2::DOSFORM;
        }
        Uc2::Command *command;
        try {
            command = new Uc2::Command(input, parseOptions);
        } catch (std::string e) {
            std::cerr << e << '\n';
            return 1;
        } URLCMD_CATCHDEFAULT;
        if (command->err()) {
            delete command;
            std::cerr << "Invalid command: " << input << '\n';
            if (options.mSkip) {
                continue;
            } else {
                return 2;
            };
        }
        std::vector<Uc2::Query> queries;
        try {
            queries = command->queries();
        } catch (std::string e) {
            std::cerr << e << '\n';
            return 1;
        } URLCMD_CATCHDEFAULT;
        std::cout << '\n';
        for (Uc2::Query query : queries) {
            if (options.mVerbosity)
                std::cout << '\t' << query.format(parseOptions) << '\n';
        }
        Uc2::Builder *builder;
        try {
            builder = new Uc2::Builder(*command, parseOptions);
        } catch (std::string e) {
            std::cerr << e << '\n';
            return 1;
        } URLCMD_CATCHDEFAULT;
        std::string cmdStr = builder->generate(parseOptions);
        std::cout << "> " << cmdStr << '\n';
        delete builder;
        delete command;
        if (!options.mPreview) {
            int exitCode = system(cmdStr.c_str());
            if (exitCode) {
                std::cerr
                    << "Command exited with nonzero exit code "
                    << exitCode <<
                    ". Propagating exit code.\n";
                    return exitCode;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int status = run(argc, argv);
    std::cout << '\n';
    return status;
}
