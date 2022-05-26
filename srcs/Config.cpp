#include "Config.hpp"

//  function:
//    [get_server_configuration]
//  parameters:
//    std::ifstream & (Configuration File)
//  description:
//    std::map: <key, value> => [directive, content].
//    Read the configuration file, parse one 'server' directive scope and return an std::map accordingly.
//    If there are no more server directives configurations to read it will return 0(ZERO).
//  example:
//    server {
//    	...	<-- reading and parsing this...
//    	...	<-- reading and parsing this ...
//    	...	<-- reading and parsing this ...
//    }
//    1.- Read line untill 'server' directive is found, else return 0.
//    2.- Parse all the content inside the 'server' scope.
//    3.-
//
//
//    Return Example:
//      map_list = [
//        ["server_name", "www.ggg.com ggg.com"],
//        ["listen", "8484"]
//      ]
//  errors:
//    Throw invalid fd exception if File Descriptor is invalid.
//    Throw unknown directive exception if an unknkown directive is founded.
//    Throw wrong syntax exception if directive is delared wrongfully.
//  notes:
//    Visit [www.github.com/] to see a list and description of the directives.
//std::map<std::string, >get_server_configuration(ostream &file) {
	// TODO Parse one server configuration file
//}

Config::Config(const Config& param) {
	// TODO (copy constructor)
	(void)param;
}

Config::~Config() {
	std::cout << "Config" << " destroyed" << std::endl;
	// TODO (destructor)
}

std::ostream& operator<<(std::ostream& s, const Config& param) {
	 s << "Some configuration Text";
	(void)param;
	return (s);
}

/* EXCEPTIONS */
const char * Config::InvalidDirectiveException::what() const throw() {
	return ("Directive is invalid");
}

const char * Config::WrongSyntaxException::what() const throw() {
	return ("Wrong Directive Syntax");
}

const char * Config::InvalidConfigurationFileException::what() const throw() {
	return ("Invalid File, expecting extention .conf and a valid file with read permission");
}

/* DirectiveContet */
/* Class to represent the content of a Directive */
Config::ServerConfig::DirectiveContent::~DirectiveContent() {
	std::cout << "DirectiveContent" << " destroyed" << std::endl;
}
