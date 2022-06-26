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


// Response::Response() {
// 	// _codeMessage.insert(std::pair<int, std::string>(400, "Bad Request"))
// 	_codeMessage.insert(std::pair<int, std::string>(200, "OK"));
// 	_codeMessage.insert(std::pair<int, std::string>(404, "Not Found"));
// 	_codeMessage.insert(std::pair<int, std::string>(413, "Bad Request"))
// 	_codeMessage.insert(std::pair<int, std::string>(414, "Bad Request"))
// 	_codeMessage.insert(std::pair<int, std::string>(431, "Bad Request"))
// 	_codeMessage.insert(std::pair<int, std::string>(400, "Bad Request"))
// 	_codeMessage.insert(std::pair<int, std::string>(505, "Bad Request"))
// }

// Response::Response(const Response& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

Response::Response(Request const & request) {
	(void)request;
	_status_code = 200; // this parametre will be defined by response and got by request.getResponse(); 
	_date = "Date: Mon, 18 Jul 2016 16:06:00 GMT";
	_server_name = "Server: BPT server 1.0";
	_content_length = "Content-Length: 176"; // + request.getLength
	_content_type = "text/html";
	_content = "\n<!DOCTYPE html><html>\n<head>\n<style type=\"text/css\" src=\"/some.css\"></style>\n</head>\n<body>\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n</body>\n</html>\n";
}

Response::~Response() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
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



// 	std::vector<std::string>::iterator it = _methods.find(request.getMethod());
// 	std::string _response;
// 	if (it == _methods.end()) {
// 		std::string line;
// 		std::fstream file;

// 		file.open("/errors/404.html", std::ios::in); // read from file
// 		if (file.is_open())
// 			while (std::getline(file, line))
// 				;
// 		int contentLength = line.length();
		
// 		_response += "HTTP/1.1 404 " + _codeMessage[404] + "\r\n";
// 		_response += "Content-Type: text/html\r\n";
// 		_response +=  "Content-Length: " + std::to_string(contentLength) + "\r\n";
// 		_response += "\r\n";
// 		_response += line;

// 		return (_response);
// 	}
// 	else if (if requested location is supported)
// 		if yes -> search for a file and return correct headder WITH A FILE

// 		else
// 			retrurn error 404 file not

// 	if (requets.getMethod)
// }

