//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Request.hpp"

Request::Request() : _method("GET"), _yuri("/content") {
	// TODO (default constructor)
}

/*Request::Request(std::string& buffer) :
_method("GET"), _yuri("/content") {

}*/

Request::Request(const Request& param) {}
		
//Request::Request(Socket const & s): _s(s){
//	TODO (default constructor)
//}

//Request::Request(const Request& param) {
	// TODO (copy constructor)
//	(void)param;
//}

Request::~Request() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

Socket const & Request::getSocket() const {
	return (_s);
}

//Request& Request::operator= (const Request& param) {
	// TODO (Assignment operatior)
	// std::swap()
//	(void)param;
//	return (*this);
// }

std::ostream& operator<<(std::ostream& s, const Request& param) {
	s << "(" << param.getSocket().getSocketFd() << ") " << param.getSocket().getIpAddress() << ":" << param.getSocket().getPort();
	(void)param;
	return (s);
}

