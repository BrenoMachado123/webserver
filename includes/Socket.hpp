#ifndef __SOCKET_HPP__
# define __SOCKET_HPP__

#include <iostream>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Config.hpp"
#include "webserv.hpp"

/************************Socket***************************/
/* This object represents a single server configuration  */
/* from the configuraton file. Listens to port & address */
/*********************************************************/
class Socket {
	private:
		int _socket_fd;
    	int _addrlen;
		int _port;
		std::string _ip_address;
    	struct sockaddr_in _address;
		Config::ServerConfig _server_config;
		Socket();
	public:
		Socket(const std::string &, int, Config::ServerConfig const &);
		~Socket();
		Socket(const Socket &);
		int acceptConnection();
		
		int getPort() const;
		int getSocketFd() const;
		int getAddressLen() const;
		Config::ServerConfig const & getServerConfig() const;

		const std::string & getIpAddress() const;
		struct sockaddr_in getAddress() const;
};
std::ostream&	operator<<(std::ostream&, const Socket&);

#endif
