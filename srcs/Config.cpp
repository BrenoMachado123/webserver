#include "Config.hpp"

/* Initialize static class members */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "autoindex"};
const std::string Config::_listen_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "error_page"};


//  function:
//    [get_server_configuration]
//  parameters:
//  description:
//    std::vector: => [directive, content].
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
//    Throw unknown directive exception if an unknkown directive is founded.
//    Throw wrong syntax exception if directive is delared wrongfully.
//  notes:
//    Visit [https://github.com/BrenoMachado123/webserver/blob/main/README.md] to see a list and description of the directives.

void	Config::get_server_configuration() throw(InvalidDirectiveException) {
	//TODO Parse one server configuration file
	ServerConfig s;
	std::cout << "Configuration File Parsed succesfully!" << std::endl;
}

// CONSTUCTORS & DESTRUCTORS //
Config::Config(std::ifstream & file) throw(InvalidConfigurationFileException): _config_file(file) {
	if (!file.is_open())
		throw e_invalid_configuration_file;
	std::cout << "Config file created!" << std::endl;
	get_server_configuration();
}

Config::~Config() {
	std::cout << "Config" << " destroyed" << std::endl;
}

Config::ServerConfig::Directive::~Directive() {
	std::cout << "DirectiveContent" << " destroyed" << std::endl;
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

/* HELPER FUNCTIONS */






















std::ostream& operator<<(std::ostream& s, const Config& param) {
	 s << "Some configuration Text";
	(void)param;
	return (s);
}


/*********26/05/2022*********/

static int checkBrackets(std::string line) {
    std::string::iterator it = line.begin();
    for (; it != line.end(); it++) {
        if (*it == '{' or *it == '}')
            return (1);
    }
    return (0);
}

void Config::checkScopes() throw(WrongSyntaxException) {
    std::string line;
    short brackets_parse = 0;
    while (std::getline(_config_file, line)) {
        std::cout << line << std::endl;
        brackets_parse += checkBrackets(line);
    }
    _config_file.close();
    if (brackets_parse % 2 != 0)
    	throw e_wrong_syntax;
    std::cout << "Status: All brackets closed" << std::endl;
    return ;
}

/****************************/
