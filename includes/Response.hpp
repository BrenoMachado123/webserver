//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>

class Response;
#include <map>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <ostream>
#include <string>

//==============//
#include <algorithm>
//==============//

#include "Request.hpp"
#include "webserv.hpp"

class Response {
	public:
		Response(Request const &);
		~Response();

		std::string createResponse();
	
	private:
		Response();
		Response(const Response&);

	private:
		int _status_code;
		std::string _date;
		std::string _server_name;
		std::string _content_length;
		std::string _content_type;
		std::string _content;
		static std::map<int, std::string> _codeMessage;
		//static std::string _mime_type_detector(std::string const & file_name);
};

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

