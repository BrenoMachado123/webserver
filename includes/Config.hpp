#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include "colors.hpp"
#include <iostream>
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


# define SERVER_CONTEXT_DIRECTIVES 2
# define LISTEN_CONTEXT_DIRECTIVES 2
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

# define PORT_MAX       65535
# define PORT_MIN       1

# define EXIT_SUCCESS   0


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
						int getId() const;
						virtual const std::string & getName() const = 0;
						virtual ~Directive();
				};

				class Root: public Directive {
					private:
						const std::string & _path;
						const std::string _name;
						Root();
					public:
						Root(const std::string &) throw (InvalidDirectiveException);
						bool _validPath(const std::string&);
						~Root();
						virtual const std::string & getName() const;
						const std::string & getPath() const;
				};

                class ErrorCodePage: public Directive {
                private:
                    ErrorCodePage();

                    std::string                             _name;
                    std::map<std::vector<int>, std::string> _errorCodePage;
                public:
                    ErrorCodePage(std::string & content) throw (InvalidDirectiveException);
                    ~ErrorCodePage();

                    const std::string & getName() const;
                    const std::map<std::vector<int>, std::string> & getErrorCodePage() const;

                    bool isCodeValid(const int &);
                    bool isStringValid(const std::string &);
                };

                class Listen: public Directive {
                private:
                    const std::string _name;
                    std::string _ip;
                    int _port;
                    Listen();
                public:
                    Listen(const std::string &) throw (InvalidDirectiveException);
                    ~Listen();

                    bool isStringValid(const std::string &);
                    bool isIpValid(const std::string &);
                    const std::string &getName() const;
                    const int &getPort() const;
                    const std::string &getIp() const;


                };

				class Methods : public Directive {
					public:
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

				void setRoot(const Root &);
                void setErrorCodePage(const ErrorCodePage &);
                void setListen(const Listen&);
                void setMethods(const Methods&);
				std::string & getRoot();
                std::map<std::vector<int>, std::string> & getErrorCodePage();
                int & getListenPort();
                std::string & getListenIp();
				std::vector<std::string>& getMethods();
			private:
				ServerConfig(const ServerConfig &);
				ServerConfig & operator=(const ServerConfig &);
				std::string                 _address;
				std::string                 _root;
                std::map<std::vector<int>, std::string> _errorCodePage;
                std::string                 _ip;
                int                         _port;
				std::vector<std::string>    _methods;
				std::vector<Directive>      _directives;
		};

		std::vector<ServerConfig> _servers;
		std::ifstream & _config_file;

		Config();
		Config & operator=(const Config &);
		Config(const Config &);
		void	get_server_configuration() throw(InvalidDirectiveException);
		void	checkScopes() throw(WrongSyntaxException);
	public:
		static const std::string _server_directives[SERVER_CONTEXT_DIRECTIVES];
		static const std::string _listen_directives[LISTEN_CONTEXT_DIRECTIVES];

		Config(std::ifstream &) throw(InvalidConfigurationFileException);
		~Config();

	/* MEMBER FUNCTIONS */

};

std::ostream&	operator<<(std::ostream&, const Config&);

#endif

