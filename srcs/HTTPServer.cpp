#include "HTTPServer.hpp"
#include <unistd.h>

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
		std::pair<int, std::vector<Client> > p(s.getSocketFd(), std::vector<Client>());
		_clients.insert(p);
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "HTTPServer created" << ENDC << std::endl;
}

HTTPServer::~HTTPServer() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
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

//void HTTPServer::addClientAt(int);

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
	Client c(conn_sock, *it);
	if (_clients.find(fd) == _clients.end())
		return ;
	_clients.find(fd)->second.push_back(c);
}

void HTTPServer::run() {
	int nfds, n;
	struct epoll_event events[MAX_EVENTS];

	std::vector<Socket>::iterator it;
	std::map<int, std::vector<Client> >::iterator m_it;
	std::cout << CYAN << std::endl << "=> Booting webserv" << std::endl;
	std::cout << "=> HTTP server starting" << std::endl;
	std::cout << "=> Run `./webserv server --help` for more startup options" << std::endl;
	std::cout << "Starting with single thread mode..." << std::endl;
	std::cout << "* Version: 1.0 (c++98) (\"TBP's Version\")" << std::endl;
	std::cout << "*          PID: " << getpid() << std::endl;
	for (it = _sockets.begin(), m_it = _clients.begin(); it != _sockets.end() && m_it != _clients.end(); it++, ++m_it) {
		std::cout << CYAN << "* Listening on " << *it << " " << PURPLE << m_it->first << " => #Clients: " << m_it->second.size() << ENDC << std::endl;;
	}
	std::cout << "[" << timestamp_in_ms() << "]" << " Use Ctrl-C to stop" << std::endl;
	for (;;) {
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
		   perror("epoll_wait");
		   exit(EXIT_FAILURE);
		}
		for (n = 0; n < nfds; ++n) {
			if (isSocketFd(events[n].data.fd)) {
				std::cout << "[" << timestamp_in_ms() << "]" << GREEN << " ServerSocket Accepting Connection" << ENDC << std::endl;
				acceptConnectionAt(events[n].data.fd);
				for (it = _sockets.begin(), m_it = _clients.begin(); it != _sockets.end() && m_it != _clients.end(); it++, ++m_it) {
					std::cout << CYAN << "* Listening on " << *it << " " << PURPLE << m_it->first << " => #Clients: " << m_it->second.size() << ENDC << std::endl;;
				}
			} else {
				std::cout << "[" << timestamp_in_ms() << "]" <<  " Some input or output was detected" << std::endl;
				int fd = events[n].data.fd;
				{
					// TEST TO SEND A RANDOM HEADER WITH HTML
					char buffer[30000] = {0};
					int valread = read(fd, buffer, 30000);
					if (valread < 0) {
						perror("In Read");
						exit(EXIT_FAILURE);
					}
					printf("READ: %d\n%s\n", valread, buffer);
					char headers[] = "HTTP/1.1 200 OK\nDate: Sun, 26 Jun 2022 13:44:15 GMT\nServer: BTP/1.0.\nContent-Length: 178\nContent-Type: text/html; charset=iso-8859-1\n";
					char html[] = "\n<!DOCTYPE html><html>\n<head>\n<style type=\"text/css\" src=\"/some.css\"></style>\n</head>\n<body>\n<h1>My First Heading</h1>\n<p>My first paragraph.</p>\n</body>\n</html>\n";
					write(fd, headers, strlen(headers));
					write(fd, html, strlen(html));
				}
				{
					// IMAGINE THAT WE UST CLOSE CONNECTION... THEN WE DO THIS:
					int big_sock(0);
					std::vector<Client>::iterator v_it;
					for (m_it = _clients.begin(); m_it != _clients.end() && big_sock == 0; ++m_it) {
						for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
							if (v_it->getFd() == events[n].data.fd) {
								big_sock = m_it->first;
						 		std::cout << "Client: " << v_it->getFd() << " " << v_it->getSocket() << std::endl;
								break;
						 	}
						}
					}
					_clients.find(big_sock)->second.erase(v_it);
					close(fd);
				}
			/****************/
			/*PSEUDO CODE****/
			/****************/
				std::string _response;
				Request r;
				Response res;
				_response = res.createResponse(r, _config);
				write(fd, _response, strlen(_response));
				printf("------------------Response message sent-------------------\n");
				close(fd);
			/****************/
			/*end: PSEUDO CODE****/
			/****************/
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
