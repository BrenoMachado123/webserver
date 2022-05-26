//****************************************************//
// Private default constructor and assign operator.   //
// To restrict their ussage outside this object scope.//
// (Not implemented, since we don't need them!)       //
//****************************************************//

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <iostream>
#include <exception>
#include <map>
#include <vector>
#include <string>
#include <fstream>


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


// Config represents the configuration file
class Config {
	// ServerConfig represents a server configuration
	class ServerConfig {
		public:
			class Directive {
				private:
					int _id;
					Directive();
				public:
					Directive(std::string const & name, std::string const & content);
					virtual ~Directive();
			};
			class Location: public Directive {
				std::vector<Directive> _location_directives;
				public:
					Location(std::string const &);
			};
			class ServerName: public Directive {
				public:
					std::vector<std::string> _names;
			};
			class ClientMaxBody: public Directive {
				public:
					int _max;
			};
			class Root: public Directive {
				public:
					int _port;
			};
			class Listen: public Directive {
				public:
					int _port;
				Listen(std::string const &);
			};
			// TO DO, IMPLEMENT ALL THE DIRECTIVES LEFT

			//Directive const & getListen();
			// Directive const & getAutoIndex();
			// Directive const & getCGI();
			// Directive const & getCGIBIN();
			// Directive const & getListen();


		private:
			std::vector<Directive> _directives;
	};

	private:
		std::vector<ServerConfig> _servers;
		std::ifstream & _config_file;
		
		Config();
		Config & operator= (const Config &);
		Config(const Config &);
		void	get_server_configuration() throw(InvalidDirectiveException);
		void	checkScopes() throw(WrongSyntaxException);
	public:
		Config(std::ifstream &) throw(InvalidConfigurationFileException);
		~Config();

	/* MEMBER FUNCTIONS */

	/* EXCEPTIONS */

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

};

std::ostream&	operator<<(std::ostream&, const Config&);

#endif

