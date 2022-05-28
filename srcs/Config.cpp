#include "Config.hpp"

/* Initialize static class members */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "lsiten"};
const std::string Config::_listen_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "index"};

/* Exceptions */
const char * Config::InvalidDirectiveException::what() const throw() {
	return ("Directive is invalid");
}
const char * Config::WrongSyntaxException::what() const throw() {
	return ("Wrong Directive Syntax");
}
const char * Config::InvalidConfigurationFileException::what() const throw() {
	return ("Invalid File, make sure you have permissions, that the file exists and the extension is .conf");
}

//TODO Parse one server configuration file
void	Config::get_server_configuration() throw(InvalidDirectiveException) {
	ServerConfig s;
	std::cout << YELLOW << "Server Root Configuration: " << s.getRoot() << ENDC << std::endl;
	ServerConfig::Root r("/some_valid/path");
	s.setRoot(r);
	std::cout << YELLOW << "Server Root Configuration: " << s.getRoot() << ENDC << std::endl;
	std::cout << YELLOW << "Configuration File Parsed succesfully!" << ENDC << std::endl;
}

// CONSTUCTORS & DESTRUCTORS //
Config::Config(std::ifstream & file) throw(InvalidConfigurationFileException): _config_file(file) {
	if (!file.is_open())
		throw e_invalid_configuration_file;
	std::cout << WHITE << "Config created" << ENDC << std::endl;
	get_server_configuration();
}

Config::~Config() {
	std::cout << RED << "Config" << " destroyed" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig(): _port(80), _address("127.0.0.1"), _root("html/") {
	std::cout << WHITE << "ServerConfig created" << ENDC << std::endl;
}

Config::ServerConfig::~ServerConfig() {
	std::cout << RED << "ServerConfig destroyed" << ENDC << std::endl;
}

Config::ServerConfig::Directive::Directive(int id): _id(id) {
	std::cout << WHITE << "Directive(" << id <<") created" << ENDC << std::endl;
}

Config::ServerConfig::Directive::~Directive() {
	std::cout << RED << "Directive" << " destroyed" << ENDC << std::endl;
}

Config::ServerConfig::Root::Root(const std::string & str) throw (InvalidDirectiveException): Directive(ROOT), _path(str), _name("root") {
	/*
	This constructor takes only one string which should be a valid path
	Ex:
		Root("/etc/www/root") [VALID]
		Root("/etc trash wrong") [throw InvalidDirectiveException]
		Root("") [throw InvalidDirectiveException]
	*/
	std::cout << WHITE << "Root created" << ENDC << std::endl;
}

Config::ServerConfig::Root::~Root() {
	std::cout << RED << "Root Directive destroyed!" << ENDC << std::endl;
}


/* ServerConfig Member Functions */

void Config::ServerConfig::setRoot(const Root & root) {
	_root = root.getPath();
}

std::string & Config::ServerConfig::getRoot() {
	return _root;
}

/* Directives Member Functions */

int Config::ServerConfig::Directive::getId() const {
	return (_id);
}

const std::string & Config::ServerConfig::Root::getName() const {
	return (_name);
}

const std::string & Config::ServerConfig::Root::getPath() const{
	return _path;
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
