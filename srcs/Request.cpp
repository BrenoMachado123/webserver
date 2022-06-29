#include "Request.hpp"

Request::Request(std::string const & request, Config::ServerConfig const & sc): _error_code(0), _serverConfig(sc) {
	std::stringstream ss(request);
	std::string line;
	
	std::getline(ss, line);
	line = strtrim(line, " \r\t");
	_method = line.substr(0, line.find_first_of(" \r\t"));
	_target = line.substr(_method.length(), line.find_last_of(" \r\t") - _method.length());
	_http_version = line.substr(_method.length() + _target.length());
	_target = strtrim(_target, " \r\t");
	_http_version = strtrim(_http_version, " \r\t");
	std::cout << PURPLE << "Method => [" << _method << "], Target => [" << _target << "], HTTP Version => [" << _http_version << "]" << ENDC << std::endl;;	
	while (std::getline(ss, line) && line != "\r\n") {
		if (line.find(':') != std::string::npos) {
			std::string name(line.substr(0, line.find(':')));
			std::string content(line.substr(line.find(':')));
    		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			_headers[name] = strtrim(content, ": \t");
			std::cout << BLUE << name << " => " << _headers[name] << ENDC << std::endl;
		}
	}
	if (_headers.find("content-length") != _headers.end()) {
		; // TODO 
	}
	if (_headers.find("cockies") != _headers.end()) {
		; // TODO
	}

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

std::string const & Request::get_target() const {
	return _target;
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

