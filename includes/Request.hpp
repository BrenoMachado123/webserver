//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>

class Request {
	public:
		Request();
		Request(const Request&);
		~Request();
		Request&	operator= (const Request&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Request&);

#endif

