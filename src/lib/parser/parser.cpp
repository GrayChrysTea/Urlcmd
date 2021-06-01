#include <urlcmd/parser.hpp>
#include <iostream>

namespace UcPsr = Urlcmd::Parser;

UcPsr::Parser::Parser(void) noexcept :
    mCommand(""),
    mResult(0),
    mIndex(0),
    mUrlPart(0)
{}

UcPsr::Command::Command(
    const std::string _command,
    UcPsr::Options &_options
) noexcept :
    mCommand(_command),
    mResult(0),
    mIndex(0),
    mUrlPart(0)
{
    if (_options.verbosity >= 1) {
        std::cout
            << "[Urlcmd::Parser::Command::Command] "
            << "Creating a parser for command: "
            << mCommand
            << "\n";
    }
}

UcPsr::Command &UcPsr::Command::clear(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Command::clear] "
            << "Clearing command parser.\n";
    }
    mCommand = "";
    return this->reset(_options);
}

UcPsr::Command &UcPsr::Command::reset(UcPsr::Options &_options) {
    if (_options.verbosity >= 3) {
        std::cout
            << "[Urlcmd::Parser::Command::reset] "
            << "Resetting command parser.\n";
    }
    mResult.clear();
    mIndex = 0;
    mUrlPart = 0;
    return *this;
}
