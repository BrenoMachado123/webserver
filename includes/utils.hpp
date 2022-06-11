#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include "/home/przemek/42/webserver/includes/webserv.hpp"

# define SEPARATORS " \t\v\n\r\f"

std::string & strtrim(std::string & str, const char * separators = SEPARATORS);

int whatServer(Epoll & ep, int const n);

//void serverResponse(Config & cf, Epoll & ep, int const & n);

#endif
