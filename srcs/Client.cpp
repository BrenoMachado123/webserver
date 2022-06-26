//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Client.hpp"

Client::Client(int fd, Socket & s): _fd(fd), _socket(s) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client" << " created" << std::endl;
}

Client::Client(const Client& param): _fd(param._fd), _socket(param._socket) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client Copy" << " created" << std::endl;
}

Client::~Client() {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Client" << " destroyed" << std::endl;
	// TODO (destructor)
}

Client& Client::operator= (const Client& param) {
	_fd = param._fd;
	_socket = param._socket;
	return (*this);
}

int Client::getFd() const {
	return	(_fd);
}

Socket const & Client::getSocket() const {
	return	(_socket);
}

std::ostream& operator<<(std::ostream& s, const Client& param) {
	s << param.getFd();
	return (s);
}

