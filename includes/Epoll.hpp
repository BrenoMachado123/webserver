#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <vector>
#include <exception>

#include <sys/epoll.h>
#include <unistd.h>

#include "Config.hpp"
#include "Server.hpp"

class Epoll {
private:
    class	EpollFailed: public std::exception {
    public:
        virtual const char * what() const throw();
    }	e_epoll_failed;

private:
    Epoll(Epoll &);
    Epoll &operator=(Epoll &);

private:
    std::vector<Server> _serverSockets;

    int _epollfd;
    int _nfds;
    struct epoll_event _epollev, _epollevent[MAX_EVENTS];

public:
    Epoll();
    ~Epoll();


    void addSocket(Server const &s);
    void epollCreate(int const & size) throw (EpollFailed);
    void epollControl(int const & flag, int const &) throw (EpollFailed);
    void epollWait() throw (EpollFailed);


    int getnfds();
    int whatServer(int const);
    std::vector<Server> getServerSocket();
    struct epoll_event getEpollEvent(int const);
};

#endif