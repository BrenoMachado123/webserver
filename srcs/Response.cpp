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

static std::string const _mime_type_detector(std::string const & file_name) {
	size_t pos(file_name.find_last_of('.'));
	if (pos == std::string::npos)
		return "text/html";
	std::string ext = file_name.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::ft_tolower);
//TEXT
	if(ext == "txt")
		return "text/plain";
	else if(ext == "html")
		return "text/html";
	else if(ext == "css")
		return "text/css";
	else if(ext == "js") //javascript // "; parametre" - charset=/anything/ makes it invalid!
		return "text/javascript";
	else if(ext == "json")
		return "application/json";
	else if(ext == "jsonld")
		return "application/ld+json";
	else if(ext == "xml")
		return "application/xml";
	else if(ext == "pdf")
		return "application/pdf";
	else if(ext == "doc") //DOCUMENTS
		return "application/msword";
	else if(ext == "docx")
		return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if(ext == "ppt")
		return "application/vnd.ms-powerpoint";
	else if(ext == "pptx")
		return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	else if(ext == "odt")
		return "application/vnd.oasis.opendocument.text";
	else if(ext == "xls")
		return "application/vnd.ms-excel";
	else if(ext == "xlsx")
		return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if(ext == "odp")
		return "application/vnd.oasis.opendocument.presentation";
	else if(ext == "ods")
		return "application/vnd.oasis.opendocument.spreadsheet";
	
	//IMAGE
	else if(ext == "jpeg" || ext == "jpg")
		return "image/jpeg";
	else if(ext == "png")
		return "image/png";
	else if(ext == "apng")
		return "image/apng";
	else if(ext == "avif")
		return "image/avif";
	else if(ext == "gif")
		return "image/gif";
	else if(ext == "svg")
		return "image/svg+xml";
	else if(ext == "webp")
		return "image/webp";
	else if(ext == "bmp")
		return "image/bmp";
	else if(ext == "ico || cur")
		return "image/x-icon";
	else if(ext == "tif" || ext == "tiff")
		return "image/tiff";

	//SOUND
	else if(ext == "mp3")
		return "audio/mpeg";
	else if(ext == "aac")
		return "audio/aac";
	else if(ext == "wav")
		return "audio/wave";

	//VIDEO
	else if(ext == "flac")
		return "audio/flac";
	else if(ext == "mpeg")
		return "audio/mpeg";
	else if(ext == "mp4")
		return "video/mp4";
	else if(ext == "avi")
		return "video/x-msvideo";
	
	//AUDIO-VIDEO
	else if(ext == "3gp")
		return "video/3gpp; audio/3gpp"; // - audio if file does not contain video
	
	//ARCHIVES
	else if(ext == "bz")
		return "application/x-bzip";
	else if(ext == "bz2")
		return "application/x-bzip2";
	else if(ext == "gz")
		return "application/gzip";
	else if(ext == "zip")
		return "application/zip";
	else if(ext == "7z")
		return "application/x-7z-compressed";
	else if(ext == "tar")
		return "application/x-tar";

	//DEFAULT    h
	else // else if(file_name == "bin") - any kind of data
		return "application/octet-stream"; // default for binary files. It means unknown binary file
}

Response::Response(Request const & request, Config::ServerConfig const & sc): _keep_alive(true),  _autoindex(false), _req(request), _server_config(sc) {
	std::ifstream file;
	std::string tmp_buffer;
	std::string buffer;
	std::string location;
	std::string extension;
	std::vector<std::string>::iterator i_it;
	
	_status_code = _req.get_error_code();
	_date = get_local_time();
	_server_name = "Breno_Tony_Pulga";
	if (_status_code == 0 && _req._loc) {
		_status_code = 404;
		if (_req.is_target_dir()) {
			if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << WHITE << "Try Index: ";
			for(i_it = _req._loc->_indexes.begin(); i_it != _req._loc->_indexes.end() && !_content.length(); ++i_it) {
				location = _req.get_final_path() + *i_it;
				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
    				std::cout << *i_it << " ";
				file.open(location.c_str(), std::ifstream::binary);
				if(file.is_open()) {
    				if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << GREEN << "[founded]";
					_status_code = 200;
					while (std::getline(file, tmp_buffer)) // PLS REFACTOR THIS, READ FULL FILE AT ONCE OR WITH A BUFFER.... ALSO PUT THIS INSIDE A FUNCTION AND REUSE THE LINES OF CODES WHICH ARE THE SAME BELLOW
						buffer += tmp_buffer + "\n";
					_content_length = buffer.length();
					_content_type = _mime_type_detector(location);
					_content = buffer;
				}
			}
			if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << ENDC << std::endl;
			if (_req._loc->_autoindex && !_content.length()) {
				_status_code = 200;
				_autoindex = true;
			}
		} else {
			location = _req.get_final_path();
			file.open(location.c_str(), std::ifstream::binary);
			if(file.is_open()) {
				_status_code = 200;
				while (std::getline(file, tmp_buffer))
					buffer += tmp_buffer + "\n";
				_content_length = buffer.length();
				_content_type = _mime_type_detector(location);
				_content = buffer;
			}
			else
				_status_code = 404;
			if (!_content.length())
				_status_code = 404;
		}
	}
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Response Created " << ENDC << std::endl;
}

Response::~Response() {
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Response" << " destroyed" << std::endl;
	// TODO (destructor)
}


// bool Response::_check_default_error_code_and_assign_path(int code) {
// 	std::map<std::string, std::vector<int> >::const_iterator it_m = _server_config._server_errors_map.begin();
// 	std::vector<int>::const_iterator it_v;
// 	for (; it_m != _server_config._server_errors_map.end() ; it_m++) {
// 		for (it_v = (it_m->second).begin() ; it_v != it_m->second.end() ; it_v++) {
// 			if (*it_v == code) {
// 				_default_error_path = it_m->first;
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }

std::string Response::createResponse() {
	std::string response;
	std::string html_content;
	std::ostringstream so;
	so << _status_code;
	if (_status_code == 200 && _req._loc) {
		if (_content.length() > 0) {
			html_content = _content;
		} else if (_autoindex) {
			struct dirent * de;
		    struct stat st;
		    struct tm tm_time;
		    DIR *dr = opendir(_req.get_final_path().c_str());
			if (dr == NULL) {
				_status_code = 404;
			} else {
				html_content = "<html>\n<head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta content=\"utf-8\" http-equiv=\"encoding\"><title>autoindex</title><style>div { display: flex; flex-wrap: wrap; justify-content: space-between; max-width: 80%;}</style></head>\n<body>\n";
				while ((de = readdir(dr)) != NULL) {
					std::string file_path(_req.get_final_path() + "/" + de->d_name);
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
	if (_status_code != 200) {
		_keep_alive = false;
		if (_req._loc) {
			; // SEARCH IF THE ERROR IS HERE IN THE LOCATION MAP
			//_req._loc.error_map
		}
		if (!html_content.length()) {
			;// SAERCH HERE IN CASE THERE IS NO LOCATION OR ERROR AND HTML_CONTENTN IS 0
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
