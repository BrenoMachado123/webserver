#include "utils.hpp"
#include "Epoll.hpp"

std::string & strtrim(std::string & str, const char * separators) {
    str.erase(str.find_last_not_of(separators) + 1);
    str.erase(0, str.find_first_not_of(separators));
    return (str);
}

int whatServer(Epoll & ep, int const n) {
    for (unsigned int i = 0 ; i < ep.getServerSocket().size() ; i++)
        if (ep.getEpollEvent(n).data.fd == ep.getServerSocket().at(i).getSocket()) {
            std::cout << "SERVER SOCKET: " << ep.getServerSocket().at(i).getSocket() << "pos: " << i << std::endl;
            return i;
        }
    return -1;
}

void serverResponse(Config & cf, Epoll & ep, int const & n) {
    char buffer[10000] = {0};
    std::cout << "PROBLEM: " << std::endl << std::endl;
    if (read(ep.getEpollEvent(n).data.fd, buffer, 10000) < 0) {
        perror("READ: ");
        throw;
    }
    std::cout << buffer << std::endl;
    if (send(ep.getEpollEvent(n).data.fd,cf.getResponse().back().c_str(),
             cf.getResponse().back().size(),0) < 0) {
        perror("SEND: ");
        throw;
    }
}