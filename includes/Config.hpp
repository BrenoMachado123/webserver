#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include "colors.hpp"
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <ctype.h>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <algorithm>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

# define AUTOINDEX		1
# define CGI			2
# define CGIBIN			3
# define CLIENTMAXBODYSIZE	4
# define ERRORPAGE		5
# define LIMITMETHODS		6
# define LISTEN			7
# define LOCATION		8
# define ROOT			9
# define SERVERNAME		10
# define UPLOAD			11
# define RETURN_D		12

# define SERVER_CONTEXT_DIRECTIVES 6
# define LOCATION_CONTEXT_DIRECTIVES 5
# define ALL_ERROR_CODES    40

# define PORT_MAX       65535
# define PORT_MIN       1

# define MAX_EVENTS     10

#include "/home/przemek/42/webserver/includes/Server.hpp"
#include "/home/przemek/42/webserver/includes/Epoll.hpp"
#include "/home/przemek/42/webserver/includes/utils.hpp"

// Config represents the configuration file
class Config {
	private:
		/* CONFIG EXCEPTIONS */
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
		/* ServerConfig is an object to represent the configuration file */
		class ServerConfig {
			public:
				class Directive {
					private:
						int _id;
                        //maybe add the string of all codes here?
						Directive();
					public:
						Directive(int);
						virtual ~Directive();
                        //It is impossible to create getDirective if its virtual
						//virtual const std::string & getName() const = 0;
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
						virtual const std::string & getName() const;
						const std::string & getPath() const;
				};

                class ErrorCodePage: public Directive {
	                private:
	                	static const int _allErrorCodes[ALL_ERROR_CODES];
	                    const std::string         _name;
	                    std::vector<int>    _errorCodes;
	                    std::string         _errorPath;
	                    ErrorCodePage();
	                    bool isCodeValid(const std::string &);
	                    bool isStringValid(const std::string &);

	                public:
	                    ErrorCodePage(const std::string & content) throw (InvalidDirectiveException);
	                    ~ErrorCodePage();
	                    const std::string & getName() const;
	                    const std::string & getErrorPath() const;
	                    const std::vector<int> & getErrorCodes() const;
                };

                class Listen: public Directive {
	                private:
	                    const std::string   _name;
	                    std::string         _ip;
	                    int                 _port;
						Listen();
	                    bool isIpValid(const std::string &);
	                public:
	                    Listen(const std::string &) throw (InvalidDirectiveException);
	                    ~Listen();
	                    const std::string &getName() const;
	                    int getPort() const;
	                    const std::string &getIp() const;
                };

				class Methods : public Directive {
					public:
						static const std::string _valid_methods[3];
						Methods(const std::string&) throw (InvalidDirectiveException);
						~Methods();
						std::vector<std::string> getMethods() const;
						virtual const std::string& getName() const;
					private:
						Methods();
						bool _validMethod(const std::string&);
						const std::string _name;
						std::vector<std::string> _methods;
				};

				ServerConfig();
				~ServerConfig();
                ServerConfig(const ServerConfig &);
                ServerConfig & operator=(const ServerConfig &);
                void setRoot(const Root &);
                void setErrorCodePage(const ErrorCodePage &);
                void setListen(const Listen&);
                void setMethods(const Methods&);
				std::string & getRoot();
                std::vector<int> & getErrorCodes();
                std::string & getErrorPath();
                int & getListenPort();
                std::string & getListenIp();
				std::vector<std::string>& getMethods();

                //std::vector<Directive> getDirective();
                //std::vector<Directive> findDirective(int);

        private:
				std::string                 _address;
				std::string                 _root;
                int                         _port;
                std::string                 _name;
                std::vector<int>            _errorCodes;
                std::string                 _errorPath;
                std::string                 _ip;
				std::vector<std::string>    _methods;
		};
// how server will access private variable of config?!
// we need to instantiate config object in get_server_configuration ?
// then from this position we will be able to access all the data
// for each server config via functions specified in public part of
// ServerConfig?
		std::vector<ServerConfig> _servers;
        std::vector<std::string> _responses;
		std::ifstream & _config_file;

		Config();
		Config & operator=(const Config &);
		Config(const Config &);
		void	parseConfiguration() throw(InvalidDirectiveException);
		void	checkScopes() throw(WrongSyntaxException);
		// bool	findServerContext(const std::string &) const;
		// bool	contextEnd(const std::string &) const;
		bool	validDirective(const std::string &, const std::string *, int len) const;
	    void    createDirective(const std::string &, const std::string &);
    public:
		static const std::string _server_directives[SERVER_CONTEXT_DIRECTIVES];
		static const std::string _location_directives[LOCATION_CONTEXT_DIRECTIVES];

		Config(std::ifstream &) throw(InvalidConfigurationFileException);
		~Config();

        std::vector<std::string>  getResponse();
        std::vector<ServerConfig> getServer();

	/* MEMBER FUNCTIONS */

};

std::ostream&	operator<<(std::ostream&, const Config&);

#endif

