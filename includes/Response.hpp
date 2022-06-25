//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>

class Response {
	public:
		Response();
		Response(const Response&);
		~Response();
		Response&	operator= (const Response&); // const for safety... not super nesessary

		std::string createResponse(const Request&);
};

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

