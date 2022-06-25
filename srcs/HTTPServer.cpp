#include "HTTPServer.hpp"

HTTPServer::HTTPServer(std::string const & file): _config(file) {
	_epollfd = epoll_create(10);
	if (_epollfd == -1) {
	   perror("epoll_create");
	   exit(EXIT_FAILURE);
	}
	std::vector<Config::ServerConfig> servers = _config._servers;
	std::vector<Config::ServerConfig>::iterator it;
	for (it = servers.begin(); it != servers.end(); ++it) {
		Socket s(it->getIp(), it->getPort());
		addSocket(s);
	}
	std::cout << WHITE << "HTTPServer created" << ENDC << std::endl;
}

HTTPServer::~HTTPServer() {
	std::cout << RED << "HTTPServer" << " destroyed" << ENDC << std::endl;
}

void HTTPServer::addSocket(Socket & s) {
	struct epoll_event ev;
	
	ev.events = EPOLLIN;
	ev.data.fd = s.getSocketFd();
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, s.getSocketFd(), &ev) == -1) {
	   perror("epoll_ctl: listen_sock");
	   exit(EXIT_FAILURE);
	}
	_sockets.push_back(s);
}

bool HTTPServer::isSocketFd(int fd) {
	std::vector<Socket>::iterator it;
	std::vector<Socket>::iterator end = _sockets.end();
	for (it = _sockets.begin(); it != end; it++) {
		if ((*it).getSocketFd() == fd)
			return (true);
	}
	return (false);
}

void HTTPServer::acceptConnectionAt(int fd) {
	int conn_sock;
	struct epoll_event ev;
	std::vector<Socket>::iterator it;
	std::vector<Socket>::iterator end = _sockets.end();

	for (it = _sockets.begin(); it != end; it++) {
		if ((*it).getSocketFd() == fd) {
			conn_sock = (*it).acceptConnection();
			if (conn_sock == -1) {
			   perror("accept");
			   exit(EXIT_FAILURE);
			}
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = conn_sock;
			if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
			   perror("epoll_ctl: conn_sock");
			   exit(EXIT_FAILURE);
			}
			break ;
		}
	}
}

void HTTPServer::run() {
	int nfds, n;
	struct epoll_event events[MAX_EVENTS];

	std::cout << YELLOW << "WebServer Information: " << ENDC << std::endl;
	std::vector<Socket>::iterator it;
	std::vector<Socket>::iterator end = _sockets.end();
	for (it = _sockets.begin(); it != end; it++) {
		std::cout << *it << std::endl;
	}
	
	for (;;) {
		std::cout << "Epoll Waiting . . ." << std::endl;
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
		   perror("epoll_wait");
		   exit(EXIT_FAILURE);
		}
		for (n = 0; n < nfds; ++n) {
			std::cout << BLUE << "[*" << n << "*] " << ENDC;
			if (isSocketFd(events[n].data.fd)) {
				std::cout << "Accepting Connection" << std::endl;
				acceptConnectionAt(events[n].data.fd);
			} else {
				std::cout << "Some input or output was detected" << std::endl;
				int fd = events[n].data.fd;
				char buffer[30000] = {0};
				int valread = read(fd, buffer, 30000);
				if (valread < 0) {
					perror("In Read");
					exit(EXIT_FAILURE);
				}
				//std::string buf(buffer);
				Request r;
				Response res;
				res.createResponse(r);
				printf("READ: %d\n%s\n", valread, buffer);
				write(fd, "Hello from server", strlen("Hello from server"));
				printf("------------------Hello message sent-------------------\n");
				close(fd);
		   }
		}
	}
}

int HTTPServer::getEpollFd() const {
	return (_epollfd);
}

int HTTPServer::numSockets() const {
	return (_sockets.size());
}

std::ostream& operator<<(std::ostream& s, const HTTPServer& param) {
	s << "[epoll - " << param.getEpollFd() << "] # of sockets: " << param.numSockets();
	return (s);
}
