#include "/home/przemek/42/webserver/includes/Epoll.hpp"

const char * Epoll::EpollFailed::what() const throw() {
    return ("Epoll Failed");
}

Epoll::Epoll() {
    std::cout << WHITE << "Epoll Constructor Called" << ENDC << std::endl;
}

Epoll::~Epoll() {
    std::cout << RED << "Epoll Destructor Called" << ENDC << std::endl;
}

void Epoll::addSocket(const Server &s) {
    _serverSockets.push_back(s);
}

void Epoll::epollCreate(int const & size) throw (EpollFailed) {
    if ((_epollfd = ::epoll_create(size)) < 0) {
        perror("CREATE: ");
        throw e_epoll_failed;
    }
}

void Epoll::epollControl(int const & flag, int const & fd) throw (EpollFailed) {
    _epollev.events = EPOLLIN;
    _epollev.data.fd = fd;
        if (::epoll_ctl(_epollfd,
                    flag,
                    fd,
                    &_epollev) < 0) {
        perror("CONTROL: ");
        throw e_epoll_failed;
    }
}

void Epoll::epollWait() throw(class Epoll::EpollFailed) {
    _nfds = ::epoll_wait(_epollfd, _epollevent, MAX_EVENTS, -1);
    if (_nfds < 0) {
        perror("WAIT: ");
        throw e_epoll_failed;
    }
}

int Epoll::getnfds() {
    return _nfds;
}

std::vector<Server> Epoll::getServerSocket() {
    return _serverSockets;
}

struct epoll_event Epoll::getEpollEvent(int const n) {
    return _epollevent[n];
}

int Epoll::whatServer(int const n) {
    for (unsigned int i = 0 ; i < _serverSockets.size() ; i++)
        if (getServerSocket().at(i).getSocket() == _epollevent[n].data.fd)
            return i;
    return -1;
}