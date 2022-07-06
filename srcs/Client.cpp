#include "Client.hpp"

Client::Client(int fd, Socket & s): _fd(fd), _keep_alive(true), _time_to_die(timestamp_in_ms() + 3000), _socket(s) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client created" << std::endl;
}

Client::Client(const Client & param): _fd(param._fd), _keep_alive(param._keep_alive), _time_to_die(param._time_to_die), _socket(param._socket) {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "Client Copy created" << std::endl;
}

Client& Client::operator= (const Client& param) {
	_fd = param._fd;
	_socket = param._socket;
	_time_to_die = param._time_to_die;
	_keep_alive = param._keep_alive;
	return (*this);
}

Client::~Client() {
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << "Client destroyed" << std::endl;
}

int Client::getFd() const {
	return	(_fd); 
}

Socket const & Client::getSocket() const {
	return	(_socket);
}

void Client::disconnect() {
	_keep_alive = false;
}

void Client::handleRequest(std::string const & request) {
	uint64_t ms_s = timestamp_in_ms();
	_time_to_die = ms_s + 3000;
	Request req(request, _socket.getServerConfig());
	Response res(req, _socket.getServerConfig());
	std::string response_content(res.createResponse());
	write(_fd, response_content.c_str(), response_content.length());
	_keep_alive = res.getKeepAlive();
	std::cout << GREEN << "Completed " << res.getStatusCode() << " " << Response::_codeMessage[res.getStatusCode()] << " in " << timestamp_in_ms() - ms_s << "ms" << ENDC  << std::endl;
}

bool Client::keepAlive() const {
	return (_keep_alive);
}

uint64_t const & Client::timeToDie() const {
	return (_time_to_die);
}

std::ostream& operator<<(std::ostream& s, const Client& param) {
	s << param.getFd();
	return (s);
}
