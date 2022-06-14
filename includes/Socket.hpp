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

#include "colors.hpp"
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
		Socket();
	public:
		Socket(const std::string &, int);
		~Socket();
		Socket(const Socket &);
		int getPort() const;
		int getSocketFd() const;
		int getAddressLen() const;
		int acceptConnection();
		const std::string & getIpAddress() const;
		struct sockaddr_in getAddress() const;
};
std::ostream&	operator<<(std::ostream&, const Socket&);

#endif
