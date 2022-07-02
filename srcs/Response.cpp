#include "Response.hpp"

static std::map<int, std::string> insert_to_map() {
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
	//int pos = file_name.find_last_of('.');
	(void)file_name;
	//file_name = file_name.substr(pos, file_name.length() - pos);
	std::string ext = "html";
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
	else if(ext == "jpeg")
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

Response::Response(Request const & request, Config::ServerConfig const & sc): _keep_alive(true), _server_config(sc) {
	std::fstream file;
	std::string tmp_buffer;
	std::string buffer;
	std::string location;
	std::string extension;

	_status_code = request.get_error_code();
	_date = get_local_time();
	_server_name = "Breno_Tony_Pulga";
	std::cout << YELLOW << "Status code " << GREEN << _status_code << ENDC << std::endl;
	if (_status_code == 0) {
		location = request.get_final_path();
		// THIS ONLY WORKS IF THE REQUEST IS REQUESTING A SPECIFIC FILE, DOESN'T WORK FOR INDEX OR AUTO INDEX
		file.open(location.c_str(), std::ios::in);
		if(file.is_open()) {
			_status_code = 200;
			// check if its default;
			while (std::getline(file, tmp_buffer))
				buffer += tmp_buffer;
			_content_length = buffer.length();
			_content_type = _mime_type_detector(location);
			_content = buffer;
			std::cout << CYAN << location << _content_length << ", " << _content_type << ", " << _date << ", " << _server_name << ENDC << std::endl;

		// autoindex part;
		//	if (is_directory && autoindex) {
		//		stream = open_dir_stream(root);
		//		_content = "<html><head><title>" + _target + "</title></head><body>\n";
		//		for (every_node_in_dir()) {
		//			if (is_file)
		//				_content += "<a href=\"" + abs_path_to_file OR path_to_file + "\"> + " filename "</a>\n"
		//			else (is_dir)
		//				_content += "<a href=\"" + abs_path_to_dir OR path_to_dir + "\"> - " filename "/ </a>\n"
		//		}
		//		_content += "</body></html>"
		//	}
		}
		else
			_status_code = 404;
	}
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << RED << "FINISH RESPONSE" << ENDC << std::endl;
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
	std::ostringstream so;
	// I am concidering that it will ALLWAYS return AN ERROR for now
	if (_status_code != 200) {
		_keep_alive = false;

		so << _status_code;
		std::string html_content;
		html_content = "<html>\n<head><title>" + so.str() + "</title></head>\n<body bgcolor=\"gray\">\n<center><h1>" + so.str() + " " + _codeMessage[_status_code] + "</h1></center>\n<hr><center>brtopu/1.0</center>\n</body>\n</html>\n";
		response += "HTTP/1.1 " + so.str() + " " + _codeMessage[_status_code] + "\n";
		response += "Date: " + _date;
		response += "Server: " + _server_name + "\n";
		response += "Content-Type: text/html\n";
		so.str(std::string());
		so << html_content.length();		
		response += "Content-Length: " + so.str() + "\n"; // html_content.length.itoa,
		response += "Connection: close\n";
		response += "\r\n";
		response += html_content;
		std::cout << RED << _status_code << ", " << so.str() << "\n" << YELLOW << response << ENDC << std::endl;
	}
	return (response);
}

bool Response::getKeepAlive(void) const {
	return (_keep_alive);
}
std::ostream& operator<<(std::ostream& s, const Response& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}
