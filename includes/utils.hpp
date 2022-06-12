#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include "webserv.hpp"
#include "Config.hpp"
#include "Epoll.hpp"

std::string & strtrim(std::string & str, const char * separators = SEPARATORS);

int whatServer(Epoll & ep, int const n);

//void serverResponse(Config & cf, Epoll & ep, int const & n);

#endif
