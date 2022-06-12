#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <iostream>
#include <string>

#include "Config.hpp"
class Epoll;

std::string & strtrim(std::string & str, const char * separators = SEPARATORS);

int whatServer(Epoll & ep, int const n);

//void serverResponse(Config & cf, Epoll & ep, int const & n);

#endif
