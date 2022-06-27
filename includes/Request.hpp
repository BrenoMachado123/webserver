//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>
#include <map>

class Request;
#include "Response.hpp"
#include "Config.hpp"
#include "Socket.hpp"
#include "webserv.hpp"

class Request {

	private:
		std::map<std::string, std::string> _headers;
		int			_error_code;
	    std::string _method;
	    std::string _uri_target;
	    std::string _http_version;
	    // std::string _query;
	    // std::string _body;
	    // std::string _transfer_encoding;
	    long _content_length;

		Config::ServerConfig & _serverConfig;

		Socket _s;
		Request(const Request &);
		Request&	operator= (const Request&);
		Request();
	public:
		Request(std::string const &, Config::ServerConfig const &);
		~Request();

		int const & get_error_code() const;
		std::string const & get_method() const;
		std::string const & get_uri_target() const;
		std::string const & get_http_version() const;
		std::string const & get_content_length() const;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		// std::string & get_error_code() const;
		//Socket const & getSocket() const;

	private:
		// Request();
		// Request(const Request&);
		// Request&	operator= (const Request&); // const for safety... not super nesessary

	private:
};

std::ostream &	operator<<(std::ostream &, const Request &);

#endif

