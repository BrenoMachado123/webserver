#include "Request.hpp"

// Can we concider that the whole request is sent at once?
Request::Request(std::string const & request, Config::ServerConfig const & sc):
	_error_code(0), _server_config(sc), _server_error_codes(_server_config._server_errors_map), _loc(NULL)
	{			
	std::stringstream ss(request);
	std::string line;
	
	std::getline(ss, line);
	line = strtrim(line, " \r\t");
	_method = line.substr(0, line.find_first_of(" \r\t"));
	_uri_target = line.substr(_method.length(), line.find_last_of(" \r\t") - _method.length());
	_http_version = line.substr(_method.length() + _uri_target.length());
	std::transform(_method.begin(), _method.end(), _method.begin(), ::ft_toupper);
	std::transform(_http_version.begin(), _http_version.end(), _http_version.begin(), ::ft_tolower);
	_uri_target = strtrim(_uri_target, " \r\t");
	_http_version = strtrim(_http_version, " \r\t");
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << BLUE << "Method => [" << _method << "], Target => [" << _uri_target << "], HTTP Version => [" << _http_version << "]" << ENDC << std::endl;
	else
		std::cout << WHITE << "Started " << _method << " \"" << _uri_target << "\"" << " at " << get_local_time(); 
	while (std::getline(ss, line) && line != "\r\n") {
		if (line.find(':') != std::string::npos) {
			std::string name(line.substr(0, line.find(':')));
			std::string content(line.substr(line.find(':')));
    		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			_headers[name] = strtrim(content, ": \t");
    		if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << BLUE << name << " => " << _headers[name] << ENDC << std::endl;
		}
	}
	if (_headers.find("content-length") != _headers.end()) {
		; // TODO 
	}
	if (_headers.find("cockies") != _headers.end()) {
		; // TODO
	}
	if (_uri_target.length() > 8000)
		_error_code = 414;
	//Separate the uri, [Path, Query, Fragment]
	// CHeck if method Exists? -> Bad Request
	// Check if mandatory HEaders Exist? -> Bad Request
	// Check if content is ok?
	else {
		_loc = _server_config.findLocation(_uri_target);
		if (!_loc) {
			_error_code = 404;
			std::cout << RED << "Wrong target [" << _uri_target << "], couldn't find any configuration" << ENDC << std::endl;
		}
		else {
			_location_root = _loc->_root_path;
			_location_error_codes = _loc->_location_errors_map;
			_final_path = _location_root + _uri_target.substr(_loc->_target.length());
			std::cout << WHITE << "Final Target Path " << CYAN << "[" << _final_path << "]" << ENDC <<std::endl;
			if (!_loc->findMethod(_method))
				_error_code = 405;
			else if (_http_version.compare("http/1.1"))
				_error_code = 505;
		}
	}
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Request Created" << ENDC << std::endl;
}
Request::~Request() {
	if (_loc)
		delete (_loc);
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << RED << "Request destroyed" << std::endl;
}

int Request::get_error_code() const {
	return (_error_code);
}

bool Request::is_target_dir() const {
	if (_loc)
		if (_loc->_target == _uri_target || (*(_uri_target.end() - 1)) == '/' )
			return (true);
	return (false);
}

// std::string const & Request::get_method() const {
// 	return _method;
// }

// std::string const & Request::get_uri_target() const {
// 	return _uri_target;
// }

// std::string const & Request::get_http_version() const {
// 	return _http_version;
// }

// long const & Request::get_content_length() const {
// 	return _content_length;
// }

// std::string const & Request::get_location_root() const {
// 	return _location_root;
// }

// Config::ServerConfig const & Request::get_server_confing() const {
// 	return _server_config;
// }

std::string const & Request::get_final_path() const {
	return (_final_path);
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

