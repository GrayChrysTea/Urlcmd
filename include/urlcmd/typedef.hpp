#pragma once
#ifndef URLCMD_TYPEDEF_H
#   define URLCMD_TYPEDEF_H

#   include <cstdint>
#   include <cstddef>

#   define URLCMD_OPTION uint32_t
#   define URLCMD_CONSTOPTION static const URLCMD_OPTION
#   define URLCMD_OPTIONMAX UINT32_MAX
#   define URLCMD_OPTIONDEFAULT 0
#   define URLCMD_PTR void*

namespace UrlCmd::Typedef {
    typedef void* pointer;
    typedef char int8;
    typedef unsigned char uint8;
    typedef short int16;
    typedef unsigned short uint16;
    typedef int int32;
    typedef unsigned int uint32;
    typedef long long int64;
    typedef unsigned long long uint64;
}

#endif
