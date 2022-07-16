#include "Response.hpp"
#include "populate_utils.hpp"

const char * Response::CGIFailure::what() const throw() {return ("CGI couldn't be executed.");}

static const std::map<int, std::string> insert_to_error_map() {
	std::map<int, std::string>	_codeMessage;
	populate_error_map(_codeMessage);
	return (_codeMessage); 
}

static const std::map<std::string, std::string> insert_to_mime_map() {
	std::map<std::string, std::string> mime_map;
	populate_mime_map(mime_map);
	return (mime_map);
}

std::map<int, std::string> Response::_codeMessage = insert_to_error_map();
std::map<std::string, std::string> Response::_mime_types = insert_to_mime_map();

Response::Response(Request const & request, Config::ServerConfig const & sc): _keep_alive(true),  _autoindex(false), _cgi_response(false), _req(request), _server_config(sc) {
	std::string		location;
	std::ifstream	file;
	std::vector<std::string>::iterator	i_it;
	
	_status_code = _req.getErrorCode();
	_date = get_local_time();
	_server_name = "Breno_Tony_Pulga";
	if (_status_code == 0 && _req._loc) {
		_status_code = 404;
		if (_req.isTargetCGI()) {
			if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << YELLOW << "CGI location " << _req.getCGIFile() << std::endl;
			location = _req.getCGIFile();
			file.open(location.c_str(), std::ifstream::binary);
			if (file.is_open() && !isDirectory(location)) {
				CGIExecution cgi_script(_req, _server_config.getIp(),_server_config.getPort());
				_status_code = cgi_script.run(_content);
				if (_status_code <= 0)
					_status_code = 500;
				else if (_status_code == 200)
					_cgi_response = true;
			}
		} else if (_req.isTargetDir()) {
			if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "Try Index: ";
			for(i_it = _req._loc->_indexes.begin(); i_it != _req._loc->_indexes.end(); ++i_it) {
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
    				std::cout << *i_it << " ";
				if (_defineLocMimeType((_req.getFinalPath() + *i_it))) {
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << GREEN << "[founded]" << ENDC << std::endl;;
					break ;
				}
			}
			if (_status_code != 200) {
				if (_req._loc->_autoindex) {
					_status_code = 200;
					_autoindex = true;
				} else if (isDirectory(_req.getFinalPath())) {
					_status_code = 403;
				}
			}
		} else {
			_defineLocMimeType(_req.getFinalPath());
		}
	}
	file.close();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Response Created " << ENDC << std::endl;
}

Response::~Response() {
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Response" << " destroyed" << std::endl;
}

void Response::setMimeType(std::string const & file_name) {
	size_t pos(file_name.find_last_of('.'));

	if (pos != std::string::npos) {
		std::string ext = file_name.substr(pos + 1);
	    std::transform(ext.begin(), ext.end(), ext.begin(), ::ft_tolower);
	    if (_mime_types.find(ext) != _mime_types.end()) {
	    	_content_type = _mime_types[ext];
	    	return ;
	    } else {
			_content_type = "text/html";
	    }
	} else {
		_content_type = "application/octet-stream"; // default for binary files. It means unknown binary file
	}
}

const std::string Response::createAutoindexResponse() {
	std::string		tmp_icon, html_content;
	std::map<std::string, std::string> icons;
	std::ifstream	icon;
	DIR	*			dr;
	struct dirent *	de;
    struct stat		st;
    struct tm		tm_time;

	populate_icons_map(icons);
    dr = opendir(_req.getFinalPath().c_str());
	if (dr == NULL) {
		_status_code = 403;
	} else {
		define_html_content(html_content, _req.getFinalPath());
		while ((de = readdir(dr)) != NULL) {
			if (*de->d_name == 0 || (*de->d_name == '.' && *(de->d_name + 1) == 0))
				continue ;
			std::string file_path(_req.getFinalPath() + de->d_name);
			int fd = open(file_path.c_str(), O_RDONLY);
			if (fd < 0 || fstat(fd, &st) == -1) {
				html_content += "<div>Failed to open " + file_path + "</div>";
				continue ;
			}
			gmtime_r(&(st.st_mtim.tv_sec), &tm_time);
			std::string s_time(asctime(&tm_time));
			std::string tmp_s_time(s_time.substr(0, s_time.length() - 1));
			std::string file_name(de->d_name);
			if (S_ISDIR(st.st_mode))
				html_content += "<div><a href=\"" + file_name + "/\">" + icons["folder"]
				+ "<span class=\"file_name\">" + de->d_name + "/</span></a><span class=\"flexible\"><span>"
				+ tmp_s_time + "</span><span> - </span></span></div>";
			else {
				std::string & tmp_icon = icons["file"];
				size_t num_file_size(st.st_size);
				std::stringstream ss;
				ss << num_file_size;
				size_t pos = file_name.find(".");
				if (pos != std::string::npos) {
					std::string file_type = file_name.substr(pos);
					if (file_type == ".html") 
						tmp_icon = icons["html"];
					else if (file_type == ".css") 
						tmp_icon = icons["css"];
					else if (file_type == ".js") 
						tmp_icon = icons["js"];
					else if (file_type == ".py") 
						tmp_icon = icons["py"];
				}
				html_content += "<div><a href=\"" + file_name + "\">" + tmp_icon + "<span class=\"file_name\">" + de->d_name
					+ "</span></a><span class=\"flexible\"><span>"+ tmp_s_time + "</span><span> " + ss.str() + " Bytes</span></span></div>";
			}
			close (fd);
	    }
	    html_content += "\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
	    closedir(dr);
	}
	return (html_content);
}

const std::string Response::CGIResponse() {
	std::string response;
	size_t c_start = _content.find_first_of("\n");
	if (c_start == std::string::npos)
		c_start = 0;
	std::string tmp = _content.substr(c_start);
	tmp = strtrim(tmp);
	response = _writeResponse(tmp.length(), _content, false);
	return (response);
}


const std::string Response::createResponse() {
	std::string response;
	std::string html_content;
	std::ostringstream so;
	std::ifstream file;
	
	if (_cgi_response && _status_code == 200) {
		_keep_alive = false;
		return (CGIResponse());
	}
	if (_status_code == 200 && _req._loc) {
		if (_content.length() > 0) {
			html_content = _content;
		} else if (_autoindex) {
			html_content = createAutoindexResponse();
		}
	}
	so << _status_code;
	if (_status_code != 200) {
		_keep_alive = false;
		if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << RED << "Render Error => Looking inside error maps for custom error..." << ENDC << std::endl;
		if (_req._loc) {
			std::map<std::string, std::vector<int> >::iterator l_it;
			for (l_it = _req._loc->_location_errors_map.begin(); l_it != _req._loc->_location_errors_map.end() && html_content.empty(); ++l_it) {
				std::vector<int>::iterator e_it;
				for (e_it = l_it->second.begin(); e_it != l_it->second.end(); ++e_it) {
					if (*e_it == _status_code) {
    					if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
							std::cout << YELLOW << "Founded Error in this Error Location path: " << l_it->first << ENDC << std::endl;
						break ;
					}
				}
				if (e_it != l_it->second.end()) {
					std::string error_loc = l_it->first + so.str() + ".html";
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << YELLOW << "Try to open file: " << error_loc << ENDC << std::endl;
					file.open(error_loc.c_str(), std::ifstream::binary);
					if(file.is_open()) {
	    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
							std::cout << GREEN << "[File opened, render file to display error...]" << ENDC << std::endl;
						readFileStream(file, html_content);
						_content_type = "text/html";
					}
					break ;
				}
			}
		}
		std::map<std::string, std::vector<int> >::const_iterator l_it;
		for (l_it = _server_config._server_errors_map.begin(); l_it != _server_config._server_errors_map.end() && html_content.empty(); ++l_it) {
			std::vector<int>::const_iterator e_it;
			for (e_it = l_it->second.begin(); e_it != l_it->second.end(); ++e_it) {
				if (*e_it == _status_code) {
					if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << YELLOW << "Founded Error in this Error Server path: " << l_it->first << ENDC << std::endl;
					break ;
				}
			}
			if (e_it != l_it->second.end()) {
				std::string error_loc = l_it->first + so.str() + ".html"; // We support only html errors
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << YELLOW << "Try to open file: " << error_loc << ENDC << std::endl;
				file.open(error_loc.c_str(), std::ifstream::binary);
				if(file.is_open()) {
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << GREEN << "[File opened, render file to display error...]" << ENDC << std::endl;
					readFileStream(file, html_content);
					_content_type = "text/html";
				}
				break ;
			}
		}
		if (!html_content.length()) {
			_content_type = "text/html";
			html_content = "<html>\n<head><title>" + so.str() + "</title></head>\n<body bgcolor=\"gray\">\n<center><h1>" + so.str() + " "
			+ _codeMessage[_status_code] + "</h1></center>\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
		}
	}
	response = _writeResponse(html_content.length(), html_content, true);
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Server Response:" << std::endl << CYAN << response.substr(0, 500) << YELLOW << (response.size() >= 500 ? (" [...] \n(Showing 500 bytes max)") : "") << ENDC << std::endl;
	return (response);
}

bool Response::_defineLocMimeType(std::string location) {
	std::ifstream	file;
	file.open(location.c_str(), std::ifstream::binary);
	if (file.is_open() && !isDirectory(location)) {
		if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
			std::cout << GREEN << "[founded] " << location << ENDC << std::endl;;
		readFileStream(file, _content);
		_status_code = 200;
		setMimeType(location);
		return (true);
	}
	file.close();
	return (false);
}

std::string Response::_writeResponse(size_t content_length, std::string& content, bool keep_alive) {
	std::stringstream ss;
	ss	<< "HTTP/1.1 " << _status_code << " " <<  _codeMessage[_status_code] << "\n"
		<< "Date: " << _date
		<< "Server: " << _server_name << "\n"
		<< "Accept-Charset: utf-8\n"
		<< "Content-Type: " << _content_type << "\n"
		<< "Content-Length: " << content_length << "\n"
		<< "Connection: " << ( keep_alive ? "keep_alive" : "closed" ) << "\n"
		<<  "\r\n"
		<< content;
	return (ss.str());
}

bool Response::getKeepAlive(void) const {
	return (_keep_alive);
}

int Response::getStatusCode(void) const {
	return (_status_code);
}

std::ostream& operator<<(std::ostream& s, const Response& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}
