//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HTTPServer.hpp"

HTTPServer::HTTPServer() {
	// TODO (default constructor)
}

HTTPServer::HTTPServer(const HTTPServer& param) {
	// TODO (copy constructor)
	(void)param;
}

HTTPServer::~HTTPServer() {
	std::cout << "HTTPServer" << " destroyed" << std::endl;
	// TODO (destructor)
}

HTTPServer& HTTPServer::operator= (const HTTPServer& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const HTTPServer& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

