#include "Request.hpp"

Request::Request(std::string const & request, Config::ServerConfig const & sc): _error_code(0), _server_config(sc) {
	std::stringstream ss(request);
	std::string line;
	
	std::getline(ss, line);
	line = strtrim(line, " \r\t");
	_method = line.substr(0, line.find_first_of(" \r\t"));
	_uri_target = line.substr(_method.length(), line.find_last_of(" \r\t") - _method.length());
	_http_version = line.substr(_method.length() + _uri_target.length());
	_uri_target = strtrim(_uri_target, " \r\t");
	_http_version = strtrim(_http_version, " \r\t");
	std::cout << PURPLE << "Method => [" << _method << "], Target => [" << _uri_target << "], HTTP Version => [" << _http_version << "]" << ENDC << std::endl;;	
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
	// /content
	Config::ServerConfig::Location * tmp_loc;
	tmp_loc = _server_config.findLocation(_uri_target);
	if (!tmp_loc) {
		_error_code = 404;
		std::cout << RED << "Wrong target [" << _uri_target << "], couldn't find any configuration" << ENDC << std::endl;
	}
	else {
		_location_root = tmp_loc->_root_path;
		_final_path = _location_root + _uri_target.substr(tmp_loc->_target.length());
		std::cout << YELLOW << "Final Target Path [" << _final_path << "]" << ENDC <<std::endl;
 		delete (tmp_loc);
 	}
	//CHECK IF METHOD IS SUPPORTED
		//change it to upper case before comparing;
	for (long unsigned int i = 0 ; i < _method.length() ; i++)
		_method.at(i) = std::toupper(_method.at(i)); // c++11

	//CHECK IF HTTP PROTOCOL IS CORRECT
		//change it to lower case before comparing
	for (long unsigned int i = 0 ; i < _http_version.length() ; i++)
		 _http_version.at(i) = std::tolower( _http_version.at(i)); // c++11

	if (!_server_config.findMethod(_method) || !_http_version.compare("http/1.1"))
		_error_code = 404;

	if (_uri_target.length() > 8000)
		_error_code = 414;

	//CHECK URI: if it exist in the location and also if its not too long.
	// for (long unsigned int i = 0 ; i < _uri_uri_target.length() ; i++)
	// 	_uri_uri_target.at(i) = std::tolower(_uri_uri_target.at(i));

	// std::vector<Config::ServerConfig::Location>::iterator itl = _serverConfig.getLocations().begin();
	// for (; itl != _serverConfig.getLocations().end(); it++)
	// 	if (_uri_uri_target == itl->getLocation())
	// 		_location_root = itl->l_getRoot();

	// for (long unsigned int i = 0 ; i < _http_version.length() ; i++)
	// 	 _http_version.at(i) = std::tolower( _http_version.at(i));

	// if (itm == _serverConfig.getMethods().end() ||
	// 	itl == _serverConfig.getLocations().end() ||
	// 	_http_version.compare("http/1.1"))
	// 		_error_code = 400;
	// else if (_uri_uri_target.length() > 8000)
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
	return _server_config;
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

