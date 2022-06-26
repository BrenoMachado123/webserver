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
# define RETURN_D			12

# define SERVER_CONTEXT_DIRECTIVES		6
# define LOCATION_CONTEXT_DIRECTIVES	5
# define ALL_ERROR_CODES				40

# define PORT_MAX 65535
# define PORT_MIN 1

# define SEPARATORS " \t\v\n\r\f"

#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>

#include <stdlib.h>

#include "Socket.hpp"
#include "webserv.hpp"

// Config represents the whole configuration file //
class Config {
	public:
		/**********Config custom exceptions**********/
		/* - File is invalid                        */
		/* - Configuration file syntax is invalid   */
		/* - Wrong Directive / Wrong Scope          */
		/********************************************/
		class	InvalidDirectiveException: public std::exception {
			public:
				virtual const char * what() const throw();
		}	e_invalid_directive;
		class	WrongSyntaxException: public std::exception {
			public:
				virtual const char * what() const throw();
		}	e_wrong_syntax;
		class	InvalidConfigurationFileException: public std::exception {
			public:
				virtual const char * what() const throw();
		}	e_invalid_configuration_file;
		/*************************ServerConfig***************************/
		/* ServerConfig is an object to represent the server Directive  */
		/* We decided not to represent this as a directive but as       */
		/* a parent to all directives inside it's scope                 */
		/****************************************************************/
		class ServerConfig {
			public:
				/*********Directive**********/
				/* AbstractClass directive  */
				/* Identify, name directive */
				/* Polymorphism             */
				/****************************/
				class Directive {
					private:
						int _id;
						Directive();
					public:
						Directive(int);
						virtual ~Directive();
						virtual const std::string & getName() const = 0;
						virtual void setDirective(ServerConfig &) const = 0;
						int getId() const;
				};

				class Root: public Directive {
					private:
						const std::string & _path;
						const std::string _name;
						bool _validPath(const std::string&);
						Root();
					public:
						Root(const std::string &) throw (InvalidDirectiveException);
						~Root();
						virtual void setDirective(ServerConfig &) const;
						virtual const std::string & getName() const;
						const std::string & getPath() const;
				};


                class ErrorCodePage: public Directive {
	                private:
	                	static const int _allErrorCodes[ALL_ERROR_CODES];
	                    const std::string _name;
	                    std::vector<int> _errorCodes;
	                    std::string _errorPath;
	                    bool isCodeValid(const std::string &);
	                    ErrorCodePage();
	                public:
	                    ErrorCodePage(const std::string &) throw (InvalidDirectiveException);
	                    ~ErrorCodePage();
						virtual void setDirective(ServerConfig &) const;
	                    const std::string & getName() const;
	                    const std::string & getErrorPath() const;
	                    const std::vector<int> & getErrorCodes() const;
                };

                class Listen: public Directive {
	                private:
	                    const std::string _name;
	                    std::string _ip;
	                    int _port;
	                    bool isIpValid(const std::string &);
						Listen();
	                public:
	                    Listen(const std::string &) throw (InvalidDirectiveException);
	                    ~Listen();
						virtual void setDirective(ServerConfig &) const;
	                    const std::string & getName() const;
	                    const std::string & getIp() const;
	                    int getPort() const;
                };

				class Methods : public Directive {
					private:
						std::vector<std::string> _methods;
						const std::string _name;
						bool _validMethod(const std::string &);
						Methods();
					public:
						Methods(const std::string &) throw (InvalidDirectiveException);
						~Methods();
						virtual void setDirective(ServerConfig &) const;
						static const std::string _valid_methods[3];
						std::vector<std::string> getMethods() const;
						virtual const std::string & getName() const;
				};

                class Location : public Directive {
                public:
                    Location (std::string const &) throw (InvalidDirectiveException);
                    ~Location();
                    //Location(const Location &);
                    Location & operator=(const Location &);

                public:
                    virtual void setDirective(ServerConfig &) const;
                    virtual const std::string & getName() const;


                    std::string const & getLocation() const;

                    void l_setAutoindex(bool);
                    bool & l_getAutoindex();

                    void l_setRoot(const Root &);
                    std::string l_getRoot() const;
                    void l_setErrorCodePage(const ErrorCodePage &);
                    std::vector<int> & l_getErrorCodes();
                    std::string & l_getErrorPath();
                    void l_setMethods(const Methods&);
                    std::vector<std::string>& l_getMethods();

                private:
                    Location();
                    // Location(Location &);
                    // Location &operator=(Location &);
                private:
                    std::string					_location;
                    std::string					_name;
                    bool						_autoindex;

                    std::vector<std::string>    _l_index;
                    std::string                 _l_root;
                    std::vector<int>            _l_errorCodes;
                    std::string                 _l_errorPath;
                    std::vector<std::string>    _l_methods;
                };

				ServerConfig();
				ServerConfig(const ServerConfig &);
				~ServerConfig();

				// void setRoot(const Root &);
    //             void setErrorCodePage(const ErrorCodePage &);
    //             void setListen(const Listen&);
    //             void setMethods(const Methods&);
                 void setLocation(const Location&);
				// std::string & getRoot();
    //             std::vector<int> & getErrorCodes();
    //             std::string & getErrorPath();
					int getPort() const;
					std::string const & getIp() const;
				// std::vector<std::string>& getMethods();
                std::string& getLocation(int index);
                std::vector<Location> & getLocations();

			private:
				ServerConfig & operator=(const ServerConfig &);
				std::string                 _ip;
                int                         _port;
				// std::string                 _root;
    //             std::string                 _name;
    //             std::vector<int>            _errorCodes;
    //             std::string                 _errorPath;
				// std::vector<std::string>    _methods;
                std::vector<std::string>	_location;

                std::vector<Location>		_locations;
		};
	private:
		static const std::string _server_directives[SERVER_CONTEXT_DIRECTIVES];
		static const std::string _location_directives[LOCATION_CONTEXT_DIRECTIVES];
		bool	validDirective(const std::string &, const std::string *, int len) const;
		Config::ServerConfig::Directive * createDirective(std::string const &, std::string const &) throw(std::exception);
		Config & operator=(const Config &);
		Config();
		Config(const Config &);
	public:
		Config(std::string const &) throw(std::exception);
		~Config();
		std::vector<ServerConfig> _servers;
};
std::ostream&	operator<<(std::ostream &, const Config &);
std::ostream&	operator<<(std::ostream &, const Config::ServerConfig &);

#endif
