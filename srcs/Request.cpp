#include "Request.hpp"

// Request::Request() : _method("GET"), _uri("/content") {
// 	// TODO (default constructor)
// }

/*Request::Request(std::string& buffer) :
_method("GET"), _yuri("/content") {

}*/

// Request::Request(const Request& param) {}
		
//Request::Request(Socket const & s): _s(s){
//	TODO (default constructor)
//}

Request::Request(std::string const & _request, Config::ServerConfig const & sc): _serverConfig(sc) {
	//PARSE INSIDE THE CONSTRUCTOR - just parse the first line METHOD TARGET VERSION
		// check if VALID METHOD, TARGET EXIST, VERSION IS 1.1
		// METHOD TARGET HTTP <- if not this order and space/tab then error
		// possible solution: 3 substrings, begining-1st SEPARATOR and chck if it is valid
	// headders are map<key, value>
	(void)_request;
}


Request::~Request() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
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

