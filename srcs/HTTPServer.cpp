#include "HTTPServer.hpp"

const char * HTTPServer::AcceptException::what() const throw() {return ("Failed to accept a connection from the Socket");}
const char * HTTPServer::EpollAddException::what() const throw() {return ("Epoll Failed to add a file descriptor");}
const char * HTTPServer::EpollCreateException::what() const throw() {return ("Epoll Failed to return a file descriptor");}
const char * HTTPServer::EpollWaitException::what() const throw() {return ("Epoll Failed to wait and return the events");}
const char * HTTPServer::ReadFdException::what() const throw() {return ("Failed to Read the event file descriptor");}

HTTPServer::HTTPServer(std::string const & file) throw (std::exception) : _config(file) {
	_epollfd = epoll_create(10);
	if (_epollfd == -1)
	   throw EpollCreateException();
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
		} catch (std::exception & e) {
			std::cout << YELLOW << "[FAILED] " << RED << e.what() << ENDC << std::endl;
			std::cout << YELLOW << "This Server Configuration contains errors, or an invalid [ip_address:port]. Please review the configuration file" << ENDC << std::endl;
		}
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << WHITE << "HTTPServer created" << ENDC << std::endl;
}

HTTPServer::~HTTPServer() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << RED << "HTTPServer" << " destroyed" << ENDC << std::endl;
}

void HTTPServer::acceptConnectionAt(int fd) throw (std::exception) {
	int								conn_sock;
	struct epoll_event				ev;
	std::vector<Socket>::iterator	it;

	for (it = _sockets.begin(); it != _sockets.end(); it++) {
		if ((*it).getSocketFd() == fd) {
			conn_sock = (*it).acceptConnection();
			if (conn_sock == -1)
			   throw AcceptException();
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = conn_sock;
			if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
			   throw EpollAddException();
			break ;
		}
	}
	Client c(conn_sock, *it);
	if (_clients.find(fd) != _clients.end())
		_clients.find(fd)->second.push_back(c);
}

void HTTPServer::addSocket(Socket & s) throw (std::exception) {
	struct epoll_event	ev;
	
	ev.events = EPOLLIN;
	ev.data.fd = s.getSocketFd();
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, s.getSocketFd(), &ev) == -1)
		throw EpollAddException();
	_sockets.push_back(s);
}

int HTTPServer::getEpollFd() const {
	return (_epollfd);
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

int HTTPServer::numSockets() const {
	return (_sockets.size());
}

void HTTPServer::run() {
	int												n;
	int												nfds;
	int												big_sock;
	struct epoll_event								events[MAX_EVENTS];
	std::vector<Socket>::iterator					it;
	std::vector<Client>::iterator					v_it;
	std::map<int, std::vector<Client> >::iterator	m_it;

	std::cout << CYAN << std::endl << "=> Booting webserv" << std::endl;
	std::cout << "=> HTTP server starting" << std::endl;
	std::cout << "=> Run `./webserv server --help` for more startup options" << std::endl;
	std::cout << "Starting with single thread mode..." << std::endl;
	std::cout << "* Version: 1.0 (c++98) (\"TBP's Version\")" << std::endl;
	std::cout << "*          PID: " << getpid() << std::endl;
	for (it = _sockets.begin(), m_it = _clients.begin(); it != _sockets.end() && m_it != _clients.end(); it++, ++m_it)
		std::cout << CYAN << "* Listening on " << *it << " " << PURPLE << m_it->first << " => #Clients: " << m_it->second.size() << ENDC << std::endl;;
	timestamp_in_ms();
	std::cout << WHITE << "Use Ctrl-C to stop" << std::endl;
	for (;;) {
		nfds = epoll_wait(_epollfd, events, MAX_EVENTS, 2000);
		if (nfds == -1)
		   throw EpollWaitException();
		for (n = 0; n < nfds; ++n) {
			if (isSocketFd(events[n].data.fd)) {
				if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
					std::cout << WHITE << "[" << timestamp_in_ms() << "]" << YELLOW << " ServerSocket Accepting Connection" << ENDC << std::endl;
				acceptConnectionAt(events[n].data.fd);
			} else {
				// IT'S BETTER TO HAVE A FUNCTION TO DO THIS.... NOT HERE.... findClient for example
				if (events[n].events == EPOLLIN) {
					for (m_it = _clients.begin(), big_sock = 0; m_it != _clients.end() && big_sock == 0; ++m_it) {
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
					int valread = read(v_it->getFd(), buffer, 30000);
					// SHOULDN'T READ HERE.... THE CLIENT SHOULD HANDLE ALSO THE READ AND DISCONECT ETC... PROB Disconnect function is stupid and later removed
					if (valread < 0)
						throw ReadFdException();
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


std::ostream& operator<<(std::ostream& s, const HTTPServer& param) {
	s << "[epoll - " << param.getEpollFd() << "] # of sockets: " << param.numSockets();
	return (s);
}
