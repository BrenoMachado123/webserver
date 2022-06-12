#include "Server.hpp"

const char * Server::ServerFailedToCreate::what() const throw() {
    return ("Server Failed");
}

Server::Server() {
    std::cout << "Server Created" << std::endl;
}

Server::~Server() {
    std::cout << "Server Destroyed" << std::endl;
}

void Server::socket(int domain, int type, int protocol) throw(ServerFailedToCreate) {
    if ((_socketfd = ::socket(domain, type, protocol)) < 0) {
        perror("SOCKET: ");
        throw e_server_failed;
    };
}

void Server::bind(int const &port, std::string const &ip) throw(ServerFailedToCreate){
    std::cout << "BIND: " << "PORT: " << port << "IP: " << ip << std::endl;
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addrlen = sizeof(_addr);
    if ((::bind(_socketfd, (struct sockaddr *)&_addr, _addrlen)) < 0) {
        perror("BIND: ");
        throw e_server_failed;
    }
}

void Server::listen(int backlog) {
    if ((::listen(_socketfd, backlog)) < 0) {
        perror("LISTEN: ");
        throw e_server_failed;
    }
}

void Server::accept() throw(ServerFailedToCreate) {
    _new_connection = (::accept(_socketfd,
                              (struct sockaddr*)&(_addr),
                              (socklen_t*)&(_addrlen)));
   if (_new_connection < 0) {
        perror("ACCEPT: ");
        throw e_server_failed;
    }
    fcntl(_new_connection, F_SETFL, O_NONBLOCK);
}

int Server::getSocket() {
    return _socketfd;
}

int Server::getAddrlen() {
    return _addrlen;
}


int Server::getNewConnection() {
    return _new_connection;
}

struct sockaddr_in Server::getAddr() {
    return _addr;
}