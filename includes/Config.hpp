#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# define AUTOINDEX			1
# define CGI				2
# define CGIBIN				3
# define CLIENTMAXBODYSIZE	4
# define ERRORPAGE			5
# define LIMITMETHODS		6
# define LISTEN				7
# define LOCATION			8
# define ROOT				9
# define SERVERNAME			10
# define UPLOAD				11
# define INDEX				12
# define RETURN_D			13
# define SERVER_CONTEXT_DIRECTIVES		8
# define LOCATION_CONTEXT_DIRECTIVES	8
# define TOTAL_DIRECTIVES	13
# define ALL_ERROR_CODES				40
# define PORT_MAX 65535
# define PORT_MIN 1
# define SEPARATORS " \t\v\n\r\f"

# define SERVER_CONTEXT 1
# define LOCATION_CONTEXT 2

#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <map>

#include <stdlib.h>
#include "webserv.hpp"

/**************************Config***+***************************/
/*                          -----                              */
/* Config represents the whole configuration file              */
/* + Config custom exceptions                                  */
/*  - File is invalid                                          */
/*  - Configuration file syntax is invalid                     */
/*  - Wrong Directive / Wrong Scope                            */
/*                          -----                              */
/**************************ServerConfig*************************/
/*                          -----                              */
/* ServerConfig is an object to represent the server Directive */
/* We decided not to represent this as a directive but as      */
/* a parent to all directives inside it's scope                */
/*                          -----                              */
/**************************Directive****************************/
/*                          -----                              */
/* Abstract Class Directive                                    */
/* This class is usefull because it represents the concept of  */
/* a directive and we can treat all directives as one abstract */
/* class                                                       */
/*                          -----                              */
/***************************************************************/
class Config {
	public:
		class InvalidDirectiveException: public std::exception {
			public:
				virtual const char * what() const throw();
		} e_invalid_directive;
		class WrongSyntaxException: public std::exception {
			public:
				virtual const char * what() const throw();
		} e_wrong_syntax;
		class InvalidConfigurationFileException: public std::exception {
			public:
				virtual const char * what() const throw();
		} e_invalid_configuration_file;
		class ServerConfig {
			public:
				class Directive {
					public:
						Directive(int);
						int	getId() const;
						virtual ~Directive();
						virtual void	setDirective(ServerConfig &, int) const = 0;
					private:
						int	_id;
						Directive();
				};
				class AutoIndex: public Directive {
					public:
						AutoIndex(const std::string &) throw (InvalidDirectiveException);
						~AutoIndex();
						virtual void	setDirective(ServerConfig &, int) const;	
					private:
						bool _option;
				};
				class ClientMaxBodySize: public Directive {
					public:
						ClientMaxBodySize(const std::string &) throw (InvalidDirectiveException);
						~ClientMaxBodySize();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						int	_max_size;
				};
 				class Cgi : public Directive {
					public:
						Cgi(const std::string&) throw (InvalidDirectiveException);
						~Cgi();
						std::vector<std::string> getCgi() const;
						virtual void setDirective(ServerConfig&,int) const;
					private:
						std::vector<std::string> _cgi;
						void _parseCgiContent(std::vector<std::string>&, const std::string&);
				};
				class CgiBin: public Directive {
					public:
						CgiBin(const std::string &) throw (InvalidDirectiveException);
						~CgiBin();
						virtual void setDirective(ServerConfig &, int) const;
					private:
						std::string	_path;
				};
                class ErrorCodePage: public Directive {
	                public:
	                    ErrorCodePage(const std::string &) throw (InvalidDirectiveException);
	                    ~ErrorCodePage();
	                	static const int	_allErrorCodes[ALL_ERROR_CODES];
						virtual void	setDirective(ServerConfig &, int) const;
	                private:
	                    std::vector<int>	_error_codes;
	                    std::string			_error_path;
	                    bool	isCodeValid(const std::string &);
	                    ErrorCodePage();
                };
				class Index: public Directive {
					public:
						Index(const std::string &) throw (InvalidDirectiveException);
						~Index();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						std::vector<std::string> _indexes;
						Index();
				};
				class Methods: public Directive {
					public:
						Methods(const std::string &) throw (InvalidDirectiveException);
						~Methods();
						virtual void	setDirective(ServerConfig &, int) const;
						static const std::string	_valid_methods[3];
					private:
						std::vector<std::string>	_methods;
						bool	_validMethod(const std::string &);
						Methods();
				};
                class Listen: public Directive {
	                public:
	                    Listen(const std::string &) throw (InvalidDirectiveException);
	                    ~Listen();
						virtual void	setDirective(ServerConfig &, int) const;
	                private:
	                    std::string	_ip;
	                    int			_port;
	                    bool	isIpValid(const std::string &);
						Listen();
				};
                class Location: public Directive {
	                public:
                    	Location (std::string const &) throw (InvalidDirectiveException);
                    	~Location();
	                    virtual void	setDirective(ServerConfig &, int) const;
						std::vector<std::string>	_methods;
						//std::string					_error_path;
						std::string					_root_path;
						std::string					_target;
						std::vector<std::string>	_indexes;
						int							_max_body_size;
						bool						_autoindex;
						std::vector<Cgi>			_cgi;
						std::string					_cgi_bin;
						
						// std::map<int, std::vector<std::string> > _cgi;
						// std::vector<Cgi> _cgi; <- I prefer this one;
						std::map<std::string, std::vector<int> > _location_errors_map;

						bool findMethod(std::string const &) const ;
					private:
						Location();

						// std::vector<int>			_l_errorCodes;
                };
				class Root: public Directive {
					public:
						Root(const std::string &) throw (InvalidDirectiveException);
						~Root();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						const std::string &	_path;
						Root();
				};
				class ServerName: public Directive {
					public:
						ServerName(const std::string &) throw (InvalidDirectiveException);
						~ServerName();
						virtual void	setDirective(ServerConfig &, int) const;
					private:
						std::vector<std::string>	_server_names;
						ServerName();
				};
				ServerConfig();
				// ServerConfig(const ServerConfig &);
				~ServerConfig();
				int					getPort() const;
				Location * findLocation(std::string const &) const;
				std::string const &	getIp() const;
				// void setLocation(const Location&);
				// std::string & getRoot();
				// std::vector<int> & getErrorCodes();
				// std::string & getErrorPath();
				// std::vector<std::string>& getMethods();
				// std::string& getLocation(int index);
				// std::vector<Location> & getLocations();
				bool						_autoindex;
				int							_max_body_size;
				std::string					_ip;
                int							_port;
				std::string                 _root_path;
				//std::string                 _error_path;
                std::vector<Location>		_locations;
				std::vector<std::string>	_indexes;
				std::vector<std::string>	_names;
				
				std::map<std::string, std::vector<int> > _server_errors_map;
				
				static const std::string	_valid_methods_server[3];

				// std::vector<int>            _errorCodes;
				// std::vector<std::string>    _methods;
		};
	public:
		Config(std::string const &) throw(std::exception);
		~Config();
		std::vector<ServerConfig>	_servers;
	private:
		static const std::string	_server_directives[SERVER_CONTEXT_DIRECTIVES];
		static const std::string	_location_directives[LOCATION_CONTEXT_DIRECTIVES];
		bool								validDirective(const std::string &, const std::string *, int len) const;
		Config::ServerConfig::Directive *	createDirective(std::string const &, std::string const &) throw(std::exception);
		Config();
		Config(const Config &);
};

std::ostream&	operator<<(std::ostream &, const Config &);
std::ostream&	operator<<(std::ostream &, const Config::ServerConfig &);
#endif
