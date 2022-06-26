#ifndef __HTTPSERVER_HPP__
# define __HTTPSERVER_HPP__

#define MAX_EVENTS 10

#include <iostream>
#include <vector>
#include <map>

#include <sys/epoll.h>

#include "Config.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "webserv.hpp"
#include "utils.hpp"
/*************************HTTPServer****************************/
/* The Engine of the project                                   */
/* This object represents a webserver                          */
/* The Webserver is configurated with a configuration file     */
/* Listens to more than one port                               */
/* The server is non blocking and it bounces propery if needed */
/* epoll function is used to monitor all incomming requests    */
/* The webserver is compatible with Mozila Firefox             */
/* ...                                                         */
/***************************************************************/
class HTTPServer {
	private:
		int _epollfd;
		std::vector<Socket> _sockets;
		std::map<int, std::vector<Client> > _clients;
		Config _config;
		bool isSocketFd(int);
		void acceptConnectionAt(int); 
		//void addClientAt(int);
		HTTPServer(const HTTPServer&);
	public:
		HTTPServer(std::string const &);
		~HTTPServer();
		void addSocket(Socket &);
		void run();
		int getEpollFd() const;
		int numSockets() const;
};
std::ostream&	operator<<(std::ostream&, const HTTPServer&);

#endif
