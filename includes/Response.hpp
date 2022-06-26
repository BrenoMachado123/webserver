//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>

//class Response;
#include <map>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <stdlib.h>
#include "Request.hpp"
#include "webserv.hpp"

class Response {
	public:
		Response();
		Response(const Response&);
		~Response();
		Response&	operator= (const Response&); // const for safety... not super nesessary

		std::string createResponse(const Request&);
	
	private:
		std::map<int, std::string> _codeMessage;
};

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

