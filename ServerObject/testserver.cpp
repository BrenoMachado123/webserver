#include "testserver.hpp"

my::Server::Server()
: _socketFd(-1), _serverConnected(false), _name(NULL) {}

my::Server::Server(std::string name)
: _socketFd(-1), _serverConnected(false), _name(name) {}

my::Server::~Server() {}

void my::Server::defineSocket(int domain, int type, int protocol) {
    _socketFd = socket(domain, type, protocol);
    if (_socketFd < 0) {
        std::cerr << "< Fatal Error > Socket creation failed." << std::endl;
        exit(-1);
    }
}

void my::Server::init(size_t port, int backlog) {
    if (_socketFd < 0) {
        std::cerr << "< Error > Socket not defined or failed" << std::endl;
        return ;
    }
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);

    if (bind(_socketFd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
        std::cerr << "< Fatal Error > Bind process failed." << std::endl;
        exit(-1);
    }
    if (listen(_socketFd, 128) < 0) {
        std::cerr << "< Fatal Error > Listen process failed." << std::endl;
        exit(-1);
    }

    if (!_name.size())
        _name = "Mr.Simple";
    std::cout << "Link started! Server: " << _name << std::endl;
    std::cout << "Port: " << port << " | IP Address: " << _address.sin_addr.s_addr << std::endl;
    _serverConnected = true;
}

void my::Server::connection(int buffer_size) {
    if (buffer_size <= 0) {
        std::cerr << "< Error > Invalid size of buffer" << std::endl;
        return ;
    }
    long bytes = 0;
    while (_serverConnected)
    {
        std::cout << "Initialized and ready!" << std::endl;
        int size_addr = sizeof(_address);
        int ret = accept(_socketFd, (struct sockaddr* )&_address, (socklen_t* )&size_addr);
        if (ret < 0) {
            std::cerr << "< Fatal Error > Failed to connect." << std::endl;
            exit(-1);
        }
        char buffer[buffer_size] = {0};
        bytes = read( ret, buffer, buffer_size);
        std::cout << "Server "  << _name << ":" << buffer;
        send(ret, "ONLINE!", strlen("ONLINE!"),0);
        close(ret);
    }
    close(_socketFd);
}
