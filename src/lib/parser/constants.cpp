#include <urlcmd/parser/constants.hpp>

namespace UcPsr = Urlcmd::Parser;


UcPsr::Options::Options(
    uint32_t _verbosity,
    uint32_t _quotes,
    URLCMD_OPTION _format
) :
    verbosity(_verbosity),
    useQuotes(_quotes),
    format(_format)
{}