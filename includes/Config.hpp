#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <exception>

#include <stdlib.h>


#include <memory>

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

# define SEPARATORS " \t\v\n\r\f"
# define MAX_EVENTS 10

#include "utils.hpp"
#include "colors.hpp"

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
	                    //bool isStringValid(const std::string &);

	                public:
	                    ErrorCodePage(const std::string & content) throw (InvalidDirectiveException);
	                    ~ErrorCodePage();
	                    const std::string & getName() const;
	                    const std::string & getErrorPath() const;
	                    const std::vector<int> & getErrorCodes() const;
                };

				//class Index -> all directivea are stored in ServerConfig. Then setters and
				//	getters are going to assing values

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

				// class Index;

				class Location : public Directive {
					public:
						Location (std::string const &) throw (InvalidDirectiveException);
						~Location();
						Location(const Location &);
                		Location & operator=(const Location &);

					public:
						// void setLocation()
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
						Location(Location &);
						Location &operator=(Location &);
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

			//TO IMPLEMENT!!
				ServerConfig();
				~ServerConfig();
                ServerConfig(const ServerConfig &);
                ServerConfig & operator=(const ServerConfig &);
                void setRoot(const Root &);
                void setErrorCodePage(const ErrorCodePage &);
                void setListen(const Listen&);
                void setMethods(const Methods&);
				void setLocation(const Location&);
				std::string & getRoot();
                std::vector<int> & getErrorCodes();
                std::string & getErrorPath();
                int & getListenPort();
                std::string & getListenIp();
				std::vector<std::string>& getMethods();
				std::string	& getLocation();
				//std::unique_ptr<Config::ServerConfig::Location> *getLocationPtr();
				std::vector<Location> & getLocations();

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
				std::string					_location;
				
				std::vector<Location>		_locations;
				//std::unique_ptr<Config::ServerConfig::Location>	_location;
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
		void	createLocationDirective(const std::string &, const std::string &);
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

