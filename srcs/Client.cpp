#include "Client.hpp"

Client::Client(int fd, Socket & s): _fd(fd), _socket(s), _time_to_die(timestamp_in_ms() + 10000) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client" << " created" << std::endl;
}

Client::Client(const Client& param): _fd(param._fd), _socket(param._socket), _time_to_die(param._time_to_die) {
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

uint64_t const & Client::getTimeToDie() const {
	return _time_to_die;
}


void Client::handleRequest(std::string const & request) {
	_time_to_die = timestamp_in_ms() + 10000;
	Request req(request, _socket.getServerConfig());
	// Request status? Error ? -> Generate Error Response
	// Status Perfect? Generate Response Evaluate METHOD
	//Response res(req, _socket.getServerConfig());
	// res.sendResponse()

	//std::string _response_content(res.createResponse());
	//write(_fd, _response_content.c_str(), _response_content.length());
	write(_fd, "Hello World", strlen("Hello World"));
}

std::ostream& operator<<(std::ostream& s, const Client& param) {
	s << param.getFd();
	return (s);
}

