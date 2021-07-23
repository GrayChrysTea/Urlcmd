#pragma once
#ifndef URLCMD_ERROR_HPP
#   define URLCMD_ERROR_HPP

#   define URLCMD_CATCHDEFAULT \
        catch (...) {std::cerr << "An error occurred.\n";}
#   define URLCMD_CATCHWITHMSG(msg) \
        catch (...) {str::cerr << msg;}

#endif
