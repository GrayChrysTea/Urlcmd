#pragma once
#ifndef URLCMD_UTILS_HPP
#   define URLCMD_UTILS_HPP

#   include "macros.h"
#   include <string>
#   include <vector>
#   include <iterator>
#   include <sstream>
#   include <algorithm>

namespace UrlCmd::Utils {
    template <class T>
    std::string bufferToString(
        T *start,
        T *end,
        int newline = 0,
        int tabs = 0
    ) {
        std::string sep = ",";
        URLCMD_REPEAT(newline) sep.append("\n");
        URLCMD_REPEAT(tabs) sep.append("\t");
        std::ostringstream str;
        std::copy(
            start,
            end,
            std::ostream_iterator<T>(str, sep.c_str())
        );
        std::string result;
        URLCMD_REPEAT(tabs) result.append("\t");
        result.append(str.str());
        return result;
    }

    template <class T>
    std::string vectorToString(
        std::vector<T> *vec,
        int newline = 0,
        int tabs = 0
    ) {
        std::string sep = ",";
        URLCMD_REPEAT(newline) sep.append("\n");
        URLCMD_REPEAT(tabs) sep.append("\t");
        std::ostringstream str;
        std::copy(
            vec->begin(),
            vec->end(),
            std::ostream_iterator<T>(str, sep.c_str()));
        std::string result;
        URLCMD_REPEAT(tabs) result.append("\t");
        result.append(str.str());
        return result;
    }
};

#endif
