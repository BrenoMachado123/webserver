//***************************//
//*Template by pulgamecanica*//
//***************************//
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

class Config {
	class ServerConfig {
		enum directive_enum {
			auth,
			autoindex,
			cgi,
			cgi_bin,
			error_page,
			index,
			listen,
			location,
			root,
			server_name,
			upload
		};
		private:
			class DirectiveContent {
				private:
					std::string const content;
					DirectiveContent();
				public:
					DirectiveContent(directive_enum, std::string content);
					~DirectiveContent();
			};
			std::map<directive_enum , DirectiveContent> directives;
	};

	private:
		//***************************************************//
		// Attribute member that stores ServerConfig objects //
		// Each element represents a ServerConfig            //
		//***************************************************//
		std::vector<ServerConfig> _serversConfigs;

		Config();
		Config & operator= (const Config &);
	public:
		Config(const Config &);
		~Config();

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

