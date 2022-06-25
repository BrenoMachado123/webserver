//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Response.hpp"

Response::Response() {
	// _codeMessage.insert(std::pair<int, std::string>(400, "Bad Request"))
	_codeMessage.insert(std::pair<int, std::string>(200, "OK"));
	_codeMessage.insert(std::pair<int, std::string>(404, "Not Found"));
	// _codeMessage.insert(std::pair<int, std::string>(413, "Bad Request"))
	// _codeMessage.insert(std::pair<int, std::string>(414, "Bad Request"))
	// _codeMessage.insert(std::pair<int, std::string>(431, "Bad Request"))
	// _codeMessage.insert(std::pair<int, std::string>(400, "Bad Request"))
	// _codeMessage.insert(std::pair<int, std::string>(505, "Bad Request"))
}

Response::Response(const Response& param) {
	// TODO (copy constructor)
	(void)param;
}

Response::~Response() {
	std::cout << "Response" << " destroyed" << std::endl;
	// TODO (destructor)
}

Response& Response::operator= (const Response& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Response& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

std::string Response::createResponse(Request const & request, Config const & conf) {
	std::vector<std::string>::iterator it = _methods.find(request.getMethod());
	std::string _response;
	if (it == _methods.end()) {
		char buff[30000];
		int fd;

		fd = open("../4242/errors/404.html", O_RDONLY);
		int contentLength = read(fd, buff, sizeof(buff));
		
		_response += "HTTP/1.1 404 " + _codeMessage[404] + "\r\n";
		_response += "Content-Type: text/html\r\n";
		_response +=  "Content-Length: " + std::to_string(contentLength) + "\r\n";
		_response += "\r\n";
		_response += buff;

		return (_response);
	}
	else if ()

	if (requets.getMethod)
}

