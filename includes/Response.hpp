//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>

#include "webserv.hpp"

class Response {
	public:
		Response();
		Response(const Response&);
		~Response();
		Response&	operator= (const Response&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

