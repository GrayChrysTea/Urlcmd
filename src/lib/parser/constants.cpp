#include <urlcmd/parser/constants.hpp>

namespace UcPsr = Urlcmd::Parser;

UcPsr::Options::Options(void) noexcept :
    verbosity(0),
    useQuotes(0),
    format(0)
{}

UcPsr::Options::Options(
    uint32_t _verbosity,
    uint32_t _quotes,
    URLCMD_OPTION _format
) noexcept :
    verbosity(_verbosity),
    useQuotes(_quotes),
    format(_format)
{}
