//***************************//
//*Template by pulgamecanica*//
//***************************//

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
	std::cout << _request << std::endl;

	std::stringstream ss(_request);
	std::string line;
	int big_context = 0;
	int context = -1;


//TJERE IS PROBLEM IWTH SWITHC AND WHILE. CHECK TEST SOLVE IT AND IMPLEMENT HERE.
	while (std::getline(ss, line)) {
		switch(big_context) {
			case 0:
			{
				int pos = line.find_first_of(SEPARATORS);
				if (++context == 0)
					_method = line.substr(0, pos);
				else if (++context == 1)
					_uri_target = line.substr(0, pos);
				else if (++context == 2) {
					_http_version = line.substr(0, pos);
					big_context++;
				}
				line.erase(0, pos + 1);
				continue ;
			}
			case 1:
			{
				int pos = line.find(':');
				_headers[line.substr(0, pos)] = line.substr(pos + 1, line.find('\n') - pos);
				continue ;
			}
		}
		std::cout << "METHOD: " << _method << std::endl;
		std::cout << "URI: " <<_uri_target << std::endl;
		std::cout << "HTTP: " << _http_version << std::endl;
		std::map<std::string, std::string>::iterator it = _headers.begin();
		for (; it != _headers.end() ; it++)
			std::cout << "HEADDER: -" << (*it).first << "->" << (*it).second << std::endl;
	}

	//PARSE INSIDE THE CONSTRUCTOR - just parse the first line METHOD TARGET VERSION
		// check if VALID METHOD, TARGET EXIST, VERSION IS 1.1
		// METHOD TARGET HTTP <- if not this order and space/tab then error
		// possible solution: 3 substrings, begining-1st SEPARATOR and chck if it is valid
	// headders are map<key, value>
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

