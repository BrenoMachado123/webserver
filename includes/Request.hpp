//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__

#include <iostream>
#include <map>
#include <algorithm>

#include "Config.hpp"
#include "Socket.hpp"
#include "webserv.hpp"
#include "utils.hpp"

class Request {
	private:
		std::map<std::string, std::string> _headers;
		int			_error_code;
	    long		_content_length;
	    std::string _method;
	    std::string _uri_target;
		std::string _location_root;
		std::string _final_path;
	    std::string _http_version;
		// std::string _post_content;
	    // std::string _query;
	    // std::string _body;
	    // std::string _transfer_encoding;

		Config::ServerConfig const & _server_config;

		bool _check_default_error_code_and_assign_path(int) const;

		// Socket _s;
		Request(const Request &);
		Request&	operator= (const Request&);
		Request();
	public:
		Request(std::string const &, Config::ServerConfig const &);
		~Request();

		int const & get_error_code() const;
		long const & get_content_length() const;
		std::string const & get_method() const;
		std::string const & get_uri_target() const;
		std::string const & get_http_version() const;
		std::string const & get_location_root() const;
		std::string const & get_final_path() const;
		bool const & get_is_default() const;
		std::string const & get_default_error_path() const ;

		Config::ServerConfig const & get_server_confing() const;
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

