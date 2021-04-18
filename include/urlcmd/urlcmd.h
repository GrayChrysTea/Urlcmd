#pragma once
#ifndef URLCMD_URLCMD_H
#   define URLCMD_URLCMD_H

#   include "macros.h"

#   define HAS_URLCMD 1

#   define URLCMD_MAJOR 0
#   define URLCMD_MINOR 3
#   define URLCMD_PATCH 0
#   define URLCMD_VERSION URLCMD_STR(URLCMD_MAJOR) "." \
        URLCMD_STR(URLCMD_MINOR) "." URLCMD_STR(URLCMD_PATCH)
#   define URLCMD_NAME urlcmd
#   define URLCMD_SNAME URLCMD_STR(URLCMD_NAME)
#   define URLCMD_FULLNAME URLCMD_SNAME " v" URLCMD_VERSION

#endif
