//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Client.hpp"

Client::Client(int fd): _fd(fd) {
	// TODO (default constructor)
}

// Client::Client(const Client& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

Client::~Client() {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Client" << " destroyed" << std::endl;
	// TODO (destructor)
}

// Client& Client::operator= (const Client& param) {
// 	// TODO (Assignment operatior)
// 	// std::swap()
// 	(void)param;
// 	return (*this);
// }

int Client::getFd() const {
	return	(_fd);
}

std::ostream& operator<<(std::ostream& s, const Client& param) {
	s << param.getFd();
	return (s);
}

