//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

#include <iostream>

#include "webserv.hpp"

class Client {
	private:
		int _fd;
		Client(const Client&);
		Client&	operator= (const Client&);
		Client();
	public:
		Client(int);
		~Client();
		int getFd() const;
};

std::ostream&	operator<<(std::ostream&, const Client&);

#endif

