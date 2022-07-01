#ifndef __UTILS_HPP__
# define __UTILS_HPP__


#include <iostream>
#include <sys/time.h>

#include "Config.hpp"
#include <webserv.hpp>

# define uint64_t unsigned long int

std::string & strtrim(std::string & str, const char * separators = SEPARATORS);
uint64_t	timestamp_in_ms(void);

char *itoa(int);
int ft_tolower (int);
int ft_toupper (int);

#endif
