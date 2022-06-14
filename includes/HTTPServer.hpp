//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HTTPSERVER_HPP__
# define __HTTPSERVER_HPP__

#include <iostream>


#include "Socket.hpp"

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
		std::map<int, Socket> _sockets_map;
		HTTPServer(const HTTPServer&);
	public:
		HTTPServer(const Config &);
		~HTTPServer();
		int getEpollFd() const;
};

std::ostream&	operator<<(std::ostream&, const HTTPServer&);

#endif

