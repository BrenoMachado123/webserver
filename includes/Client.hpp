#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

#include <iostream>

#include "Config.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "webserv.hpp"

class Client {
	private:
		int _fd;
		Socket & _socket;
		// Config::ServerConfig _serverConfig;
		Client();
	public:
		Client(int, Socket &);
		~Client();
		Client(const Client&);
		Client&	operator= (const Client&);
		int getFd() const;
		Socket const &  getSocket() const;
		void handleRequest(std::string const &);
};

std::ostream&	operator<<(std::ostream&, const Client&);

#endif

