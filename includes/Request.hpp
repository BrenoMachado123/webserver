//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>

class Request {
	public:
		Request();
		Request(std::string&);
		Request(const Request&);
		~Request();
		Request&	operator= (const Request&); // const for safety... not super nesessary

	private:
		std::string _method;
		std::string _yuri;
};

std::ostream&	operator<<(std::ostream&, const Request&);

#endif

