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


Request::Request(std::string const & _request, Config::ServerConfig const & sc): _error_code(0), _serverConfig(sc) {
	std::stringstream ss(_request);
	std::string line;

	// if (_request.length()) ??
	_content_length = _request.length(); //this is ALL request, we need only content!! part after \r\n
										// this content length is always defined in the headder
	std::getline(ss, line);
//WE CAN CHECK FIRST IF THERE ARE 3 TOKENS
// THEN IF THEY ARE PARSE THEm

//READ REQUEST LINE
	// if (_request.empty())
	// 	return ;
	// _method = line.substr(0,  line.find_first_of(SEPARATORS));
	// line = line.substr(line.find_first_of(SEPARATORS), line.length());
	// line = strtrim(line);
	// if (line.empty())
	// 	_error_code = 400;
	// _uri_target = line.substr(0,  line.find_first_of(SEPARATORS));
	// line = line.substr(line.find_first_of(SEPARATORS), line.length());
	// line = strtrim(line);
	// if (line.empty())
	// 	_error_code = 400;
	// _http_version = line.substr(0,  line.find_first_of(SEPARATORS));
	// line = line.substr(line.find_first_of(SEPARATORS), line.length());
	// line = strtrim(line);
	// if (!line.empty())
	// 	_error_code = 400;

//READ HEADDRES
	while (std::getline(ss, line) && line != "\r\n") {
		long unsigned int pos = line.find(':');
		if (pos == std::string::npos)
			break ;
		_headers[line.substr(0, pos)] = line.substr(pos + 1, line.find('\n') - pos);
		if (_headers[line.substr(0, pos)].length() > 8000) { //check if header isnt too long
			_error_code = 414; 
			break ;
		}
		std::cout << "infinite1" << std::endl;
	}

//READ CONTENT
	while (std::getline(ss, line)) {
		_post_content += line;
		_content_length = _post_content.length();
		std::cout << "infinite2" << std::endl;
	}

//PRINTING TO CHECK IF IT WORKS AS EXPECTED
	std::cout << "METHOD: " << _method << std::endl;
	std::cout << "URI: " << _uri_target << std::endl;
	std::cout << "HTTP: " << _http_version << std::endl;
	std::map<std::string, std::string>::iterator it = _headers.begin();
	for (; it != _headers.end() ; it++)
		std::cout << "HEADDER: -" << (*it).first << "->" << (*it).second << std::endl;


	//CHECK IF METHOD IS SUPPORTED
	// for (long unsigned int i = 0 ; i < _method.length() ; i++)
	// 	_method.at(i) = std::tolower(_method.at(i));

	// std::vector<std::string>::iterator itm = _serverConfig.getMethods().begin();
	// for (; itm != _serverConfig.getMethods().end() ; it++)
	// 	;


	//CHECK URI: if it exist in the location and also if its not too long.
	// for (long unsigned int i = 0 ; i < _uri_target.length() ; i++)
	// 	_uri_target.at(i) = std::tolower(_uri_target.at(i));

	// std::vector<Config::ServerConfig::Location>::iterator itl = _serverConfig.getLocations().begin();
	// for (; itl != _serverConfig.getLocations().end(); it++)
	// 	if (_uri_target == itl->getLocation())
	// 		_location_root = itl->l_getRoot();

	// for (long unsigned int i = 0 ; i < _http_version.length() ; i++)
	// 	 _http_version.at(i) = std::tolower( _http_version.at(i));

	// if (itm == _serverConfig.getMethods().end() ||
	// 	itl == _serverConfig.getLocations().end() ||
	// 	_http_version.compare("http/1.1"))
	// 		_error_code = 400;
	// else if (_uri_target.length() > 8000)
	// 	_error_code = 414;

	std::cout << PURPLE << "FINISHED" << ENDC << std::endl;
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

Config::ServerConfig const & Request::get_server_confing() const {
	return _serverConfig;
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

