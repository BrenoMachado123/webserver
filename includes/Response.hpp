//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>

#include <map>
#include <fstream>
#include <sstream>
#include <ostream>
#include <string>
#include <algorithm>

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>

#include "Request.hpp"
#include "webserv.hpp"
#include "utils.hpp"

class Response {
	public:
		Response(Request const &, Config::ServerConfig const & sc);
		~Response();

		std::string createResponse(void);
		bool getKeepAlive(void) const;
	private:
		Response();
		Response(const Response&);

	private:
		int _status_code;
		bool _keep_alive;
		bool _autoindex;
		std::string _date;
		std::string _server_name;
		std::string _content_length;
		std::string _content_type;
		std::string _content;
		Request const & _req;
		Config::ServerConfig const & _server_config;
		static std::map<int, std::string> _codeMessage;

		//bool _check_default_error_code_and_assign_path(int);
		//static std::string _mime_type_detector(std::string const & file_name);
};

//map<string, vecotr<int> >
/*
{
	{"path/error/", [400, 401]},
	{"ErrornewPATH/", [402, 414]},
}
*/

std::ostream&	operator<<(std::ostream&, const Response&);

#endif

