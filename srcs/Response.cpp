//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Response.hpp"

Response::Response() {
	// TODO (default constructor)
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

void Response::createResponse(const Request& request) {
	
}

