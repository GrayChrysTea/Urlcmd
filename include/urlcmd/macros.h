#pragma once
#ifndef URLCMD_MACROS_H
#   define URLCMD_MACROS_H

#   define _URLCMD_STR(token) #token
#   define URLCMD_STR(token) _URLCMD_STR(token)

#   define URLCMD_REPEAT(number) \
    for (unsigned long long _ = 0; _ < number; ++_)

#endif