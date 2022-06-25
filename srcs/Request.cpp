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

Request::Request(const Request& param) {
	// TODO (copy constructor)
	(void)param;
}

Request::~Request() {
	std::cout << "Request" << " destroyed" << std::endl;
	// TODO (destructor)
}

Request& Request::operator= (const Request& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Request& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

