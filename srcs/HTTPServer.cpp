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
		try {
			Socket s(it->getIp(), it->getPort(), *it);
			addSocket(s);
			std::pair<int, std::vector<Client> > p(s.getSocketFd(), std::vector<Client>());
			_clients.insert(p);
    		if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
				std::cout << std::endl << *it;
		} catch (std::exception &e) {
			std::cout << YELLOW << "[FAILED] " << RED << e.what() << ENDC << std::endl;
			std::cout << YELLOW << "This Server Configuration contains errors, please review the configuration file" << ENDC << std::endl;
		}
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
	timestamp_in_ms();
	std::cout << WHITE << "Use Ctrl-C to stop" << std::endl;
	for (;;) {
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, 2000);
		if (nfds == -1) {
		   perror("epoll_wait");
		   exit(EXIT_FAILURE);
		}
		for (n = 0; n < nfds; ++n) {
			if (isSocketFd(events[n].data.fd)) {
				if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << WHITE << "[" << timestamp_in_ms() << "]" << YELLOW << " ServerSocket Accepting Connection" << ENDC << std::endl;
				acceptConnectionAt(events[n].data.fd);
			} else {
				int fd = events[n].data.fd;
				if (events[n].events == EPOLLIN) {
					int big_sock(0);
					std::vector<Client>::iterator v_it;
					for (m_it = _clients.begin(); m_it != _clients.end() && big_sock == 0; ++m_it) {
						for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
							if (v_it->getFd() == events[n].data.fd) {
								big_sock = m_it->first;
								break;
						 	}
						}
					}
					if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
						std::cout << WHITE << "[" << timestamp_in_ms() << "] " << YELLOW << "Input From Client: " << v_it->getFd() << " " << CYAN << "[" << v_it->getSocket() << "]" << ENDC << std::endl;
					char buffer[30000] = {0};
					int valread = read(fd, buffer, 30000);

					if (valread < 0) {
						perror("In Read\n");
						exit(EXIT_FAILURE);
					}
					if (valread > 0) {
						std::string _buffer(buffer, valread);
						v_it->handleRequest(_buffer);
					} else {
						v_it->disconnect();
					}
				}
		   }
		}
		uint64_t timestamp(timestamp_in_ms());
		std::vector<Client>::iterator v_it;
		std::map<int, std::vector<std::vector<Client>::iterator > > _clients_to_die;

	/*
	 * @brief This function iterates via map of clients and checks if time passed is higher than "time to die" for the client
	 * 			If it is then it closes the connection with that client (that was saved in a vector) and erase it from vector.
	 */
		for (m_it = _clients.begin(); m_it != _clients.end() ; ++m_it) {
			for (v_it = m_it->second.begin(); v_it != m_it->second.end(); ++v_it) {
				if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << WHITE << "[" << timestamp << "] " << PURPLE << *v_it << " " << v_it->timeToDie() << ENDC << std::endl;
				if (v_it->timeToDie() < timestamp || !(v_it->keepAlive())) {
					_clients_to_die[m_it->first].push_back(v_it);
				}
			}
		}
		for (std::map<int, std::vector<std::vector<Client>::iterator > >::iterator c_it = _clients_to_die.begin() ; c_it != _clients_to_die.end() ; ++c_it) {
			for (std::vector<std::vector<Client>::iterator >::iterator cc_it = c_it->second.begin(); cc_it != c_it->second.end(); ++cc_it) {
				close((*cc_it)->getFd());
				_clients[c_it->first].erase(*cc_it);
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
