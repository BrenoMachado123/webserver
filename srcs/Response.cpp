#include "Response.hpp"

static const std::map<int, std::string> insert_to_map() {
	std::map<int, std::string> _codeMessage;
	_codeMessage[200] = "OK";
	_codeMessage[400] = "Bad Request";
	_codeMessage[404] = "Not Found";
	_codeMessage[405] = "Method Not Allowed";
	_codeMessage[413] = "Payload Too Large";
	_codeMessage[414] = "URI Too Long";
	_codeMessage[415] = "Unsupported Media Type";
	_codeMessage[431] = "Request Header Fields Too Large";
	_codeMessage[500] = "Internal Server Error";
	_codeMessage[505] = "HTTP Version Not Supported";

	return (_codeMessage); 
}

std::map<int, std::string> Response::_codeMessage = insert_to_map();

void Response::setMimeType(std::string const & file_name) {
	size_t pos(file_name.find_last_of('.'));
	if (pos == std::string::npos) {
		_content_type = "text/html";
		return;
	}
	std::string ext = file_name.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::ft_tolower);
//TEXT
	if(ext == "txt")
		_content_type = "text/plain";
	else if(ext == "html")
		_content_type = "text/html";
	else if(ext == "css")
		_content_type = "text/css";
	else if(ext == "js") //javascript // "; parametre" - charset=/anything/ makes it invalid!
		_content_type = "text/javascript";
	else if(ext == "json")
		_content_type = "application/json";
	else if(ext == "jsonld")
		_content_type = "application/ld+json";
	else if(ext == "xml")
		_content_type = "application/xml";
	else if(ext == "pdf")
		_content_type = "application/pdf";
	else if(ext == "doc") //DOCUMENTS
		_content_type = "application/msword";
	else if(ext == "docx")
		_content_type = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if(ext == "ppt")
		_content_type = "application/vnd.ms-powerpoint";
	else if(ext == "pptx")
		_content_type = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	else if(ext == "odt")
		_content_type = "application/vnd.oasis.opendocument.text";
	else if(ext == "xls")
		_content_type = "application/vnd.ms-excel";
	else if(ext == "xlsx")
		_content_type = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if(ext == "odp")
		_content_type = "application/vnd.oasis.opendocument.presentation";
	else if(ext == "ods")
		_content_type = "application/vnd.oasis.opendocument.spreadsheet";
	
	//IMAGE
	else if(ext == "jpeg" || ext == "jpg")
		_content_type = "image/jpeg";
	else if(ext == "png")
		_content_type = "image/png";
	else if(ext == "apng")
		_content_type = "image/apng";
	else if(ext == "avif")
		_content_type = "image/avif";
	else if(ext == "gif")
		_content_type = "image/gif";
	else if(ext == "svg")
		_content_type = "image/svg+xml";
	else if(ext == "webp")
		_content_type = "image/webp";
	else if(ext == "bmp")
		_content_type = "image/bmp";
	else if(ext == "ico || cur")
		_content_type = "image/x-icon";
	else if(ext == "tif" || ext == "tiff")
		_content_type = "image/tiff";

	//SOUND
	else if(ext == "mp3")
		_content_type = "audio/mpeg";
	else if(ext == "aac")
		_content_type = "audio/aac";
	else if(ext == "wav")
		_content_type = "audio/wave";

	//VIDEO
	else if(ext == "flac")
		_content_type = "audio/flac";
	else if(ext == "mpeg")
		_content_type = "audio/mpeg";
	else if(ext == "mp4")
		_content_type = "video/mp4";
	else if(ext == "avi")
		_content_type = "video/x-msvideo";
	
	//AUDIO-VIDEO
	else if(ext == "3gp")
		_content_type = "video/3gpp; audio/3gpp"; // - audio if file does not contain video
	
	//ARCHIVES
	else if(ext == "bz")
		_content_type = "application/x-bzip";
	else if(ext == "bz2")
		_content_type = "application/x-bzip2";
	else if(ext == "gz")
		_content_type = "application/gzip";
	else if(ext == "zip")
		_content_type = "application/zip";
	else if(ext == "7z")
		_content_type = "application/x-7z-compressed";
	else if(ext == "tar")
		_content_type = "application/x-tar";

	//DEFAULT    h
	else // else if(file_name == "bin") - any kind of data
		_content_type = "application/octet-stream"; // default for binary files. It means unknown binary file
}

Response::Response(Request const & request, Config::ServerConfig const & sc): _keep_alive(true),  _autoindex(false), _req(request), _server_config(sc) {
	std::ifstream file;
	std::string tmp_buffer;
	std::string buffer;
	std::string location;
	std::string extension;
	std::vector<std::string>::iterator i_it;
	
	_status_code = _req.getErrorCode();
	_date = get_local_time();
	_server_name = "Breno_Tony_Pulga";
	if (_status_code == 0 && _req._loc) {
		_status_code = 404;
		if (_req.isTargetDir()) {
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
					_content = readFile(file);
					_status_code = 200;
					setMimeType(location);
					break ;
				}
			}
			if (_req._loc->_autoindex && _content.empty()) {
				_status_code = 200;
				_autoindex = true;
			}
		} else {
			location = _req.getFinalPath();
			file.open(location.c_str(), std::ifstream::binary);
			if(file.is_open() && !isDirectory(location)) {
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << GREEN << "[founded] " << location << ENDC << std::endl;;
				_content = readFile(file);
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

std::string Response::createResponse() {
	std::string response;
	std::string html_content;
	std::ostringstream so;
	std::ifstream file;
	
	if (_status_code == 200 && _req._loc) {
		if (_content.length() > 0) {
			html_content = _content;
		} else if (_autoindex) {
			struct dirent * de;
		    struct stat st;
		    struct tm tm_time;
		    DIR *dr = opendir(_req.getFinalPath().c_str());
			if (dr == NULL) {
				_status_code = 404;
			} else {
				html_content = "<html>\n<head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta content=\"utf-8\" http-equiv=\"encoding\"><title>autoindex</title><style>div { display: flex; flex-wrap: wrap; justify-content: space-between; max-width: 80%;}</style></head>\n<body>\n";
				while ((de = readdir(dr)) != NULL) {
					std::string file_path(_req.getFinalPath() + "/" + de->d_name);
					int fd = open(file_path.c_str(), O_RDONLY);
					if (fstat(fd, &st) == -1)
						continue ;
					gmtime_r(&(st.st_mtim.tv_sec), &tm_time);
					std::string s_time(asctime(&tm_time));
					std::string tmp_s_time(s_time.substr(0, s_time.length() - 1));
					std::string slash;
					std::string file_name(de->d_name);
					if (de->d_type == 4)
						slash = "/";
					html_content += "<div><a href=\"" + file_name + slash + "\">" + de->d_name + slash + "</a><span>" + tmp_s_time + "</span></div>";
					close (fd);
			    }
			    html_content += "\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
			    closedir(dr);
			}
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
							std::cout << GREEN << "[File opened render file to display error...]" << ENDC << std::endl;
						std::string tmp_buffer;
						while (std::getline(file, tmp_buffer)) // PLS REFACTOR THIS, READ FULL FILE AT ONCE OR WITH A BUFFER.... ALSO PUT THIS INSIDE A FUNCTION AND REUSE THE LINES OF CODES WHICH ARE THE SAME BELLOW
							html_content += tmp_buffer + "\n";
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
				std::string error_loc = l_it->first + so.str() + ".html";
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << YELLOW << "Try to open file: " << error_loc << ENDC << std::endl;
				file.open(error_loc.c_str(), std::ifstream::binary); // We support only html errors
				if(file.is_open()) {
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << GREEN << "[File opened render file to display error...]" << ENDC << std::endl;
					std::string tmp_buffer;
					while (std::getline(file, tmp_buffer)) // PLS REFACTOR THIS, READ FULL FILE AT ONCE OR WITH A BUFFER.... ALSO PUT THIS INSIDE A FUNCTION AND REUSE THE LINES OF CODES WHICH ARE THE SAME BELLOW
						html_content += tmp_buffer + "\n";
					_content_type = "text/html";
				}
				break ;
			}
		}
		if (!html_content.length())
			html_content = "<html>\n<head><title>" + so.str() + "</title></head>\n<body bgcolor=\"gray\">\n<center><h1>" + so.str() + " " + _codeMessage[_status_code] + "</h1></center>\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
	}
	response += "HTTP/1.1 " + so.str() + " " + _codeMessage[_status_code] + "\n";
	response += "Date: " + _date;
	response += "Server: " + _server_name + "\n";
	response += "Accept-Charset: utf-8\n";
	response += "Content-Type: " + _content_type + "\n";
	so.str(std::string());
	so << html_content.length();
	response += "Content-Length: " + so.str() + "\n";
	response += "Connection: close\n";
	response += "\r\n";
	response += html_content;
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Server Response:" << std::endl << CYAN << response.substr(0, 500) << YELLOW << (response.size() >= 500 ? (" [...] \n(Showing 500 bytes max)") : "") << ENDC << std::endl;
	return (response);
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
