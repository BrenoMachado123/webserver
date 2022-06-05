#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <string.h>

namespace my {
    class Server {
        public:
            Server();
            Server(std::string name);
            ~Server();
            void defineSocket(int domain, int type, int protocol);
            void init(size_t port, int backlog);
            void connection(int buffer_size);

        protected:
            int _socketFd;
            bool _serverConnected;
            std::string _name;
            struct sockaddr_in _address;
    };
}

#endif