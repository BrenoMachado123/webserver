#include "webserv.hpp"
#include "Socket.hpp"

static void help(char *prog_name) {
	std::cout << RED << "usage " << prog_name << " config_file.conf" << std::endl;
	std::cout << YELLOW << "hints: check if configuration file exists." << ENDC << std::endl;
}

#include <sys/epoll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>


#define LISTEN_BACKLOG 50
#define MAX_EVENTS 10
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define PORT 8080


/*This function will Read and write to the file discriptor in the end the connection is closed!*/
void do_use_fd(int fd) {
	char buffer[30000] = {0};
	int valread = read(fd, buffer, 30000);
	if (valread < 0) {
		perror("In Read");
        exit(EXIT_FAILURE);
	}
	printf("READ: %d\n%s\n", valread, buffer);
	write(fd, "Hello from server", strlen("Hello from server"));
	printf("------------------Hello message sent-------------------\n");
	close(fd);
}

void run() {
	struct epoll_event ev, events[MAX_EVENTS];
	int listen_sock, conn_sock, nfds, epollfd, n;
	

	Socket s1("127.0.0.98", 4242);
	std::cout << s1 << std::endl;
    /* SETUP VALUES FROM SERVER TO USE ON EPOLL */
    listen_sock = s1.getSocketFd();

	epollfd = epoll_create(10);
	if (epollfd == -1) {
	   perror("epoll_create");
	   exit(EXIT_FAILURE);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
	   perror("epoll_ctl: listen_sock");
	   exit(EXIT_FAILURE);
	}

	for (;;) {
	   std::cout << "Epoll Waiting . . ." << std::endl;
	   nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
	   if (nfds == -1) {
	       perror("epoll_wait");
	       exit(EXIT_FAILURE);
	   }

	   for (n = 0; n < nfds; ++n) {
	   		std::cout << BLUE << "[" << n << "] " << ENDC;
	       if (events[n].data.fd == listen_sock) {
	       	   std::cout << "Accepting Connection" << std::endl;
	           conn_sock = s1.acceptConnection();
	           if (conn_sock == -1) {
	               perror("accept");
	               exit(EXIT_FAILURE);
	           }
	           fcntl(conn_sock, F_SETFL, O_NONBLOCK); //setnonblocking(conn_sock);
	           ev.events = EPOLLIN | EPOLLET;
	           ev.data.fd = conn_sock;
	           if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
	                       &ev) == -1) {
	               perror("epoll_ctl: conn_sock");
	               exit(EXIT_FAILURE);
	           }
	       } else {
	       	   std::cout << "Some input or output was detected" << std::endl;
	           do_use_fd(events[n].data.fd);
	       }
	   }
	}
}



int	main(int ac, char **av)
{
	if (ac != 2)
	{
		help(av[0]);
		return (1);
	}
	std::cout << GREEN << "TESTING" << ENDC << std::endl;
	run();
	std::cout << GREEN << "FINISHED" << ENDC << std::endl;
	/*
	try {
		std::ifstream file;
		file.open(av[1], std::ios::in);
		Config c(file);
		file.close();
	} catch (std::exception & e)
 	{
		std::cout << e.what() << std::endl;
	}*/

	return (0);
}
