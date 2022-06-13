#ifndef SERVER_HPP
# define SERVER_HPP

#include <exception>
#include <string>
#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>

class Server {
private:
    class ServerFailedToCreate : public std::exception {
    public:
        virtual const char * what() const throw();
    }	e_server_failed;

private:
    int _socketfd;
    int _new_connection;
    int _addrlen;
    struct sockaddr_in _addr;
public:
    Server();
    ~Server();
    int getSocket();
    int getAddrlen();
    int getNewConnection();
    struct sockaddr_in getAddr();
    void socket(int domain, int type, int protocol) throw(ServerFailedToCreate);
    void bind(int const &port, std::string const & ip) throw(ServerFailedToCreate);
    void listen(int backlog);
    void accept() throw(ServerFailedToCreate);
};

#endif