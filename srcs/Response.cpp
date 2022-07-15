#include "Response.hpp"
#include "populate_utils.hpp"

const char * Response::CGIFailure::what() const throw() {return ("CGI couldn't be executed.");}

static void push_back_env(std::vector<char *> & vec, std::string const & name, std::string const & value) {
	if (!name.empty() && !value.empty()) {
		std::string tmp(value);
		tmp = name + "=" + strtrim(tmp);
		if (tmp.length() < 8000)
			vec.push_back(strdup(tmp.c_str()));
	}
}

static const std::map<int, std::string> insert_to_error_map() {
	std::map<int, std::string>	_codeMessage;
	populate_error_map(_codeMessage);
	return (_codeMessage); 
}

static const std::map<std::string, std::string> insert_to_mime_map() {
	std::map<std::string, std::string> mime_map;
	populate_mime_map(mime_map);
//	vec.push_back(std::make_pair("", "application/octet-stream"));
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
				_status_code = execCGI();
				if (_status_code <= 0)
					_status_code = 500;
			}
		} else if (_req.isTargetDir()) {
			if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "Try Index: ";
			for(i_it = _req._loc->_indexes.begin(); i_it != _req._loc->_indexes.end(); ++i_it) {
				location = _req.getFinalPath() + *i_it;
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
    				std::cout << *i_it << " ";
				file.open(location.c_str(), std::ifstream::binary);
				if (file.is_open() && !isDirectory(location)) {
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << GREEN << "[founded]" << ENDC << std::endl;;
					readFileStream(file, _content);
					_status_code = 200;
					setMimeType(location);
					break ;
				}
			}
				// try server indexes
			if (_status_code != 200) {
				if (_req._loc->_autoindex) {
					_status_code = 200;
					_autoindex = true;
				} else if (isDirectory(_req.getFinalPath())) {
					_status_code = 403;
				}
			}
		} else {
			location = _req.getFinalPath();
			file.open(location.c_str(), std::ifstream::binary);
			if(file.is_open() && !isDirectory(location)) {
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << GREEN << "[founded] " << location << ENDC << std::endl;;
				readFileStream(file, _content);
				_status_code = 200;
				setMimeType(location);
			}
		}
	}
	file.close();
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
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

int Response::execCGI() {
	std::vector<char *> env;
	std::vector<char *> arg;
	std::stringstream ss;

	ss << _req.getContent().length();
	std::string content_length = ss.str();
	ss.str(std::string());
	ss << _server_config.getPort();
	_populateCgiEnviroment(env, content_length, ss.str());
	arg.push_back(strdup(_req.getCGIFile().c_str()));
	arg.push_back(NULL);

	int restore_input = dup(STDIN_FILENO);
	int restore_output = dup(STDOUT_FILENO);
	FILE * tmp_file_in = tmpfile();
	FILE * tmp_file_out = tmpfile();
	if (!tmp_file_in || !tmp_file_out || restore_input < 0)
		return (-1);
    int tmp_fd_in = fileno(tmp_file_in);
    int tmp_fd_out = fileno(tmp_file_out);
	if (tmp_fd_in < 0)
		return (-1);
	write(tmp_fd_in, _req.getContent().c_str(), _req.getContent().length());
	rewind(tmp_file_in);
	int pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0) {
		dup2(tmp_fd_in, STDIN_FILENO);
		dup2(tmp_fd_out, STDOUT_FILENO);
		execve(_req.getCGIFile().c_str(), &arg[0], &env[0]);
		exit(EXIT_FAILURE);
    }
    int child_status;
	waitpid(pid, &child_status, 0);
    close(tmp_fd_in);
    rewind(tmp_file_out);
    if (child_status != 0){
    	_content = "ERROR!!!";
    	return (500);
    }
    else {
	    char buff[1024];
	    int valread = -1;
	    while(valread != 0) {
	    	bzero(buff, 1024);
			valread = read(tmp_fd_out, buff, 1024);
			if (valread < 0)
				return (-1);
			_content += buff;
	    }
	}
    close(tmp_fd_out);
    dup2(restore_input, STDIN_FILENO);
    dup2(restore_output, STDOUT_FILENO);
    close(restore_input);
    close(restore_output);
	std::cout << YELLOW << " CGI Child Finished with status: " << ENDC << child_status;
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << std::endl << "Content" << std::endl << _content << std::endl;
    _cgi_response = true;
	return (200);
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
				// I don't know how the code behaves in the old implementation using find(".something"), so if the following line cause errors, please undo it.
				size_t pos = file_name.find(".");
				if (pos != std::string::npos) {
					std::string file_type = file_name.substr(pos);
					std::cout << " --------=========>  " << file_type << std::endl;
					if (file_type == ".html") 
						tmp_icon = icons["html"];
					else if (file_type == ".css") 
						tmp_icon = icons["css"];
					else if (file_type == ".js") 
						tmp_icon = icons[".js"];
					else if (file_type == ".py") 
						tmp_icon = icons["py"];
				}
				// ----------------------------------------
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
	std::stringstream ss;

	ss << _status_code;
	response += "HTTP/1.1 " + ss.str() + " " + _codeMessage[_status_code] + "\n";
	response += "Date: " + _date;
	response += "Server: " + _server_name + "\n";
	response += "Access-Control-Allow-Origin: *\n";
	response += "Connection: close\n";
	ss.str(std::string());
	size_t c_start = _content.find_first_of("\n");
	if (c_start == std::string::npos)
		c_start = 0;
	std::string tmp = _content.substr(c_start);
	tmp = strtrim(tmp);
	ss << tmp.length();
	response += "Content-Length: " + ss.str() + "\n";
	response += _content;
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
			html_content = "<html>\n<head><title>" + so.str() + "</title></head>\n<body bgcolor=\"gray\">\n<center><h1>" + so.str() + " " + _codeMessage[_status_code] + "</h1></center>\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
		}
	}
	response += "HTTP/1.1 " + so.str() + " " + _codeMessage[_status_code] + "\n";
	response += "Date: " + _date;
	response += "Server: " + _server_name + "\n";
	response += "Accept-Charset: utf-8\n";
	response += "Content-Type: " + _content_type + "\n";
	so.str(std::string());
	so << html_content.length();
	response += "Content-Length: " + so.str() + "\n";
	response += "Connection: keep-alive\n";
	response += "\r\n";
	response += html_content;
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Server Response:" << std::endl << CYAN << response.substr(0, 500) << YELLOW << (response.size() >= 500 ? (" [...] \n(Showing 500 bytes max)") : "") << ENDC << std::endl;
	return (response);
}

void Response::_populateCgiEnviroment(std::vector<char *>& env, std::string& content_length,std::string server_port) {
	push_back_env(env, "PATH", _req.getCGIFile());
	//push_back_env(env, "AUTH_TYPE", "");
	push_back_env(env, "CONTENT_LENGTH", content_length);
	push_back_env(env, "CONTENT_TYPE", "application/x-www-form-urlencoded"); //+ _req.getContentType());
	push_back_env(env, "DOCUMENT_ROOT", _req.getCGIBinPath());
	push_back_env(env, "GATEWAY_INTERFACE", "CGI/1.1");
	push_back_env(env, "HTTP_ACCEPT", "application/x-www-form-urlencoded,text/xml,application/xml,application/xhtml+xml,text/html,text/plain,charset=utf-8;");
	//push_back_env(env, "HTTP_COOKIE", "");
	//push_back_env(env, "HTTP_PRAGMA", "");
	push_back_env(env, "HTTP_USER_AGENT", _req.getUserAgent());
	push_back_env(env, "PATH_INFO", _req.getCGIBinPath());
	push_back_env(env, "PATH_TRANSLATED", _req.getCGIBinPath());
	push_back_env(env, "QUERY_STRING", _req.getQuery());
	//push_back_env(env, "REMOTE_ADDR", "");
	push_back_env(env, "REMOTE_HOST", _server_config.getIp());
	//push_back_env(env, "REMOTE_IDENT", "");
	//push_back_env(env, "REMOTE_PORT", "");
	//push_back_env(env, "REMOTE_USER", "");
	push_back_env(env, "REQUEST_METHOD", _req.getMethod());
	push_back_env(env, "REQUEST_URI", _req.getUriTarget());
	push_back_env(env, "SCRIPT_FILENAME", _req.getCGIFile());
	push_back_env(env, "SCRIPT_NAME", _req.getCGIFile());
	push_back_env(env, "SERVER_ADMIN", "pulgamecanica11@gmail.com");
	push_back_env(env, "SERVER_NAME", "BRTOAN");
	push_back_env(env, "SERVER_PORT", server_port);
	push_back_env(env, "SERVER_PROTOCOL", "HTTP/1.1");
	push_back_env(env, "SERVER_SOFTWARE", "Webserv42.0 (Linux)");
	env.push_back(NULL);
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
