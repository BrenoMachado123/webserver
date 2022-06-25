#include "Socket.hpp"

Socket::Socket(const std::string & ip, int port) {
	_port = port;
	_ip_address = ip;
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == 0) {
        perror("Socket");
        exit(EXIT_FAILURE);
    }
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = inet_addr(_ip_address.c_str());
    if (_address.sin_addr.s_addr == INADDR_NONE) {
    	std::cout << RED << "Invalid Ip Address: " << _ip_address << ENDC << std::endl;
    	exit(EXIT_FAILURE);
    }
    _address.sin_port = htons(_port);
    _addrlen = sizeof(_address);
    bzero(_address.sin_zero, sizeof(_address.sin_zero));
    if (bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
        perror("Bind");
        exit(EXIT_FAILURE);
    }
    if (listen(_socket_fd, 10) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Socket created" << ENDC << std::endl;
}

Socket::~Socket() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Socket [" << _ip_address << ":" << _port <<"] destroyed" << ENDC << std::endl;
}

Socket::Socket(const Socket & s) {
    _socket_fd = s._socket_fd;
    _addrlen = s._addrlen;
    _port = s._port;
    _ip_address = s._ip_address;
    _address = s._address;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Copy Socket created" << ENDC << std::endl;
}

int Socket::getPort() const {
	return _port;
}

int Socket::getSocketFd() const {
	return _socket_fd;
}
int Socket::getAddressLen() const {
	return _addrlen;
}
const std::string & Socket::getIpAddress() const {
	return _ip_address;
}
struct sockaddr_in Socket::getAddress() const {
	return _address;
}

int Socket::acceptConnection() {
	return (accept(_socket_fd, (struct sockaddr *) & _address, (socklen_t *)& _addrlen));
    //fcntl(conn_sock, F_SETFL, O_NONBLOCK);
}

std::ostream& operator<<(std::ostream& s, const Socket& param) {
	s << CYAN << "Socket: http://" << param.getIpAddress() << ":" << param.getPort() << ENDC;
	return (s);
}
