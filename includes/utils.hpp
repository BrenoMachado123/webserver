#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <iostream>
# define SEPARATORS " \t\v\n\r\f"

std::string & strtrim(std::string & str, const char * separators = SEPARATORS);

#endif
