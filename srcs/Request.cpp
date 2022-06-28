//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Request.hpp"

// Request::Request() : _method("GET"), _uri("/content") {
// 	// TODO (default constructor)
// }

/*Request::Request(std::string& buffer) :
_method("GET"), _yuri("/content") {

}*/

// Request::Request(const Request& param) {}
		
//Request::Request(Socket const & s): _s(s){
//	TODO (default constructor)
//}

Request::Request(std::string const & _request, Config::ServerConfig const & sc): _serverConfig(sc), _error_code(0) {
	std::stringstream ss(_request);
	std::string line;
	int big_context = 0;
	int context = 0;

	// if (_request.length()) ??
	_content_length = _request.length();

	while (std::getline(ss, line)) {
		switch(big_context) {
			case 0:
			{
                while (!line.empty()) {
                    int pos = line.find_first_of(SEPARATORS);
                    if (context == 0)
                        _method = line.substr(0, pos);
                    else if (context == 1)
                        _uri_target = line.substr(0, pos);
                    else if (context == 2) {
                        _http_version = line.substr(0, pos);
						line.erase(0, pos);
                        big_context++;
                    }
					line.erase(0, pos + 1);
                    context++;
                }
				continue ;
			}
			case 1:
			{
				int pos = line.find(':');
				_headers[line.substr(0, pos)] = line.substr(pos + 1, line.find('\n') - pos);
				if (_headers[line.substr(0, pos)].length() > 8000) //check if header isnt too long
					_error_code = 414; break ;
				continue ;
			}
		}
    }

//PRINTING TO CHECK IF IT WORKS AS EXPECTED
	std::cout << "METHOD: " << _method << std::endl;
	std::cout << "URI: " << _uri_target << std::endl;
	std::cout << "HTTP: " << _http_version << std::endl;
	std::map<std::string, std::string>::iterator it = _headers.begin();
	for (; it != _headers.end() ; it++)
		std::cout << "HEADDER: -" << (*it).first << "->" << (*it).second << std::endl;


	//CHECK IF METHOD IS SUPPORTED
	for (int i = 0 ; i < _method.length() ; i++)
		_method.at(i) = std::tolower(_method.at(i));

	std::vector<std::string>::iterator itm = _serverConfig.getMethods().begin();
	for (; itm != _serverConfig.getMethods().end() ; it++)
		;


	//CHECK URI: if it exist in the location and also if its not too long.
	for (int i = 0 ; i < _uri_target.length() ; i++)
		_uri_target.at(i) = std::tolower(_uri_target.at(i));

	std::vector<Config::ServerConfig::Location>::iterator itl = _serverConfig.getLocations().begin();
	for (; itl != _serverConfig.getLocations().end(); it++)
		if (_uri_target == itl->getLocation())
			_location_root = itl->l_getRoot();

	for (int i = 0 ; i < _http_version.length() ; i++)
		 _http_version.at(i) = std::tolower( _http_version.at(i));

	if (itm == _serverConfig.getMethods().end() ||
		itl == _serverConfig.getLocations().end() ||
		_http_version.compare("http/1.1"))
			_error_code = 400;
	else if (_uri_target.length() > 8000)
		_error_code = 414;

	//PARSE INSIDE THE CONSTRUCTOR - just parse the first line METHOD TARGET VERSION
		// check if VALID METHOD, TARGET EXIST, VERSION IS 1.1
		// METHOD TARGET HTTP <- if not this order and space/tab then error
		// possible solution: 3 substrings, begining-1st SEPARATOR and chck if it is valid
	// headders are map<key, value>
}


Request::~Request() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

int const & Request::get_error_code() const {
	return _error_code;
}

std::string const & Request::get_method() const {
	return _method;
}

std::string const & Request::get_uri_target() const {
	return _uri_target;
}

std::string const & Request::get_http_version() const {
	return _http_version;
}

long const & Request::get_content_length() const {
	return _content_length;
}

std::string const & Request::get_location_root() const {
	return _location_root;
}

// Socket const & Request::getSocket() const {
// 	return (_s);
// }

//Request& Request::operator= (const Request& param) {
	// TODO (Assignment operatior)
	// std::swap()
//	(void)param;
//	return (*this);
// }

// std::ostream& operator<<(std::ostream& s, const Request& param) {
// 	s << "(" << param.getSocket().getSocketFd() << ") " << param.getSocket().getIpAddress() << ":" << param.getSocket().getPort();
// 	(void)param;
// 	return (s);
// }

