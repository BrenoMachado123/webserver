#include "testserver.hpp"
#include <vector>
#include <sys/epoll.h>
#include <fcntl.h>
#define MAX_EVENTS 10
#define MAX_SERVERS 2
int what_server(int fd, std::vector<my::Server> servers) {
    for (int i = 0; i < servers.size() ; i++) {
        if (fd == (servers.at(i)).getFd())
            return i;
    }
    return -1;
}
int main() {
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    int conn_sock, nfds, epollfd;
    std::vector<my::Server> servers;
    std::vector<int> ports{8080, 4242};
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    for (int i = 0; i < MAX_SERVERS; i++) {
        my::Server server;
        std::cout << &server << std::endl;
        server.defineSocket(AF_INET, SOCK_STREAM, 0);
        server.init(ports.at(i), 10);
        servers.push_back(server);
    }
    /*my::Server server("TesterTron");
    server.defineSocket(AF_INET, SOCK_STREAM, 0);
    server.init(8080, 10);
    servers.push_back(server);
    my::Server server2("TesterTron2");
    server2.defineSocket(AF_INET, SOCK_STREAM, 0);
    server2.init(4242, 10);
    servers.push_back(server2);*/
    epollfd = epoll_create(1);
    if (epollfd == -1) {
        exit(1);
    }
    for (int i = 0; i < MAX_SERVERS; i++) {
        ev.events = EPOLLIN;
        ev.data.fd = (servers.at(i)).getFd();
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, (servers.at(i)).getFd(), &ev) == -1) {
            std::cerr << "epoll_ctl()" << std::endl;
            exit(1);
        }
    }
    /*ev.events = EPOLLIN;
    ev.data.fd = server.getFd();
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server.getFd(), &ev) == -1) {
        std::cerr << "epoll_ctl()" << std::endl;
        exit(1);
    }
    ev.events = EPOLLIN;
    ev.data.fd = server2.getFd();
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server2.getFd(), &ev) == -1) {
        std::cerr << "epoll_ctl()" << std::endl;
        exit(1);
    }*/
    long bytes = 0;
    int counter = -1;
    for (;;) {
        std::cout << "Waiting for connection..." << std::endl << std::endl;
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << "epoll_wait()" << std::endl;
            exit(1);
        }
        for (int n = 0; n < nfds; n++) {
            std::cout << "initial counter : " << ++counter << std::endl;
            int pos = what_server(events[n].data.fd, servers);
            if ( pos != -1 /*events[n].data.fd == server.getFd() or events[n].data.fd == server2.getFd()*/) {
                std::cout << "Entry through if : events[n].data.fd : " << events[n].data.fd << std::endl;
                int addrlen = (servers.at(pos)).get_addrlen();
                struct sockaddr_in address = (servers.at(pos)).getaddr();
                conn_sock = accept((servers.at(pos)).getFd(), (struct sockaddr*)&(address), (socklen_t*)&(addrlen));
                if (conn_sock == -1) {
                    std::cerr << "accept()" << std::endl;
                    exit(1);
                }
                fcntl(conn_sock, F_SETFL, O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    std::cerr << "epoll_ctl()" << std::endl;
                    exit(1);
                }
            }
            else {
                std::cout << "coming from else: " << events[n].data.fd << std::endl;
                std::cout << "counter : " << counter << std::endl;
                char buffer[10000] = {0};
                bytes = read(events[n].data.fd, buffer, 10000);
                std::cout << buffer << std::endl;
                if (send(events[n].data.fd, hello.c_str(), hello.size(),0) < 0) {
                    std::cerr << "send()" << std::endl;
                    exit(1);
                }
            }
        }
    }
    return (0);
}
