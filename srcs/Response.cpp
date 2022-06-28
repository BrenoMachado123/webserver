//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Response.hpp"

static std::map<int, std::string> insert_to_map() {
	std::map<int, std::string> _codeMessage;
	_codeMessage[200] = "OK";
	_codeMessage[400] = "Bad Request";
	_codeMessage[404] = "Not Found";
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
	int pos = file_name.find_last_of('.');

	file_name.substr(pos, file_name.length() - pos);

//TEXT
	if(file_name == "txt")
		return "text/plain";
	else if(file_name == "html")
		return "text/html";
	else if(file_name == "css")
		return "text/css";
	else if(file_name == "js") //javascript // "; parametre" - charset=/anything/ makes it invalid!
		return "text/javascript";
	else if(file_name == "json")
		return "application/json";
	else if(file_name == "jsonld")
		return "application/ld+json";
	else if(file_name == "xml")
		return "application/xml";
	else if(file_name == "pdf")
		return "application/pdf";
	else if(file_name == "doc") //DOCUMENTS
		return "application/msword";
	else if(file_name == "docx")
		return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if(file_name == "ppt")
		return "application/vnd.ms-powerpoint";
	else if(file_name == "pptx")
		return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	else if(file_name == "odt")
		return "application/vnd.oasis.opendocument.text";
	else if(file_name == "xls")
		return "application/vnd.ms-excel";
	else if(file_name == "xlsx")
		return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if(file_name == "odp")
		return "application/vnd.oasis.opendocument.presentation";
	else if(file_name == "ods")
		return "application/vnd.oasis.opendocument.spreadsheet";
	
	//IMAGE
	else if(file_name == "jpeg")
		return "image/jpeg";
	else if(file_name == "png")
		return "image/png";
	else if(file_name == "apng")
		return "image/apng";
	else if(file_name == "avif")
		return "image/avif";
	else if(file_name == "gif")
		return "image/gif";
	else if(file_name == "svg")
		return "image/svg+xml";
	else if(file_name == "webp")
		return "image/webp";
	else if(file_name == "bmp")
		return "image/bmp";
	else if(file_name == "ico || cur")
		return "image/x-icon";
	else if(file_name == "tif" || file_name == "tiff")
		return "image/tiff";

	//SOUND
	else if(file_name == "mp3")
		return "audio/mpeg";
	else if(file_name == "aac")
		return "audio/aac";
	else if(file_name == "wav")
		return "audio/wave";

	//VIDEO
	else if(file_name == "flac")
		return "audio/flac";
	else if(file_name == "mpeg")
		return "audio/mpeg";
	else if(file_name == "mp4")
		return "video/mp4";
	else if(file_name == "avi")
		return "video/x-msvideo";
	
	//AUDIO-VIDEO
	else if(file_name == "3gp")
		return "video/3gpp; audio/3gpp"; // - audio if file does not contain video
	
	//ARCHIVES
	else if(file_name == "bz")
		return "application/x-bzip";
	else if(file_name == "bz2")
		return "application/x-bzip2";
	else if(file_name == "gz")
		return "application/gzip";
	else if(file_name == "zip")
		return "application/zip";
	else if(file_name == "7z")
		return "application/x-7z-compressed";
	else if(file_name == "tar")
		return "application/x-tar";

	//DEFAULT    h
	else // else if(file_name == "bin") - any kind of data
		return "application/octet-stream"; // default for binary files. It means unknown binary file
}



Response::Response(Request const & request) {
	std::fstream file;
	std::string buffer;
	std::string location;
	std::string extension;

	if(request.get_error_code()) { //otherwise its set to 0 in response
		_status_code = request.get_error_code();
		location = "../errors/" + std::to_string(_status_code) + ".html";
	}

	
	//here we need to combine the root url with the file so file.open works correctly
	int pos = request.get_uri_target().find_last_of('.');

// copies extension of the uri_target
	std::copy(request.get_uri_target().begin() + pos, request.get_uri_target().end(), extension.begin()); // copy extension and later location = root + extension;
	
// location is set to root of the directive location.
// it was determined in request.
	location = request.get_location_root() + extension;

// request error code is sent by default to 0. If there is no errors it is 0, if the error during request parsing occures it has value.
	if(!request.get_error_code())  { 
		file.open(location.c_str(), std::ios::in);
		if(!file.is_open()) {
			_status_code = 404;
			location = "../errors/" + std::to_string(_status_code) + ".html";
		}
		else {
			_status_code = 200;
			location = request.get_location_root(); // ++ need to add file?
		}
	}
	file.open(location.c_str(), std::ios::in);
	while (!std::getline(file, buffer))
		;
	_content_length = buffer.length();
	_content_type = _mime_type_detector(location); // we pass location to be trimmed inside the function
	_date = "Date: Mon, 18 Jul 2016 16:06:00 GMT"; //request.get_date();
	_server_name = "Server: BPT server 1.0";
	_content = buffer;
}

Response::~Response() {
    if(CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Response" << " destroyed" << std::endl;
	// TODO (destructor)
}


std::ostream& operator<<(std::ostream& s, const Response& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

std::string Response::createResponse() {
	std::string response;
	std::ostringstream so;

	so << _status_code;

	response += "HTTP/1.1 " + so.str() + " " + _codeMessage[_status_code] += "\r\n";
	response += _content_type;
	response += _content_length;
	response += _date;
	response += _server_name;
	response += "\r\n";
	response += _content;

	return (response);
}
