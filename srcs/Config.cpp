#include "Config.hpp"

/* Initialize static class members */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "listen", "server_name", "error_page", "client_max_body_size", "location"};
const std::string Config::_location_directives[LOCATION_CONTEXT_DIRECTIVES] = {"root", "index", "limit_methods", "autoindex", "error_page"};
const std::string Config::ServerConfig::Methods::_valid_methods[3] = {"GET", "POST", "DELETE"};
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
// CONSTUCTORS & DESTRUCTORS //
Config::Config(std::ifstream & file) throw(InvalidConfigurationFileException): _config_file(file) {
	if (!file.is_open())
		throw e_invalid_configuration_file;
	std::cout << WHITE << "Config created" << ENDC << std::endl;
	parseConfiguration();
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
	if (!str.size() or !_validPath(str))
		throw InvalidDirectiveException();
	std::cout << WHITE << "Root created" << ENDC << std::endl;
}
Config::ServerConfig::Root::~Root() {
	std::cout << RED << "Root Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Methods::Methods(const std::string& content) throw (InvalidDirectiveException): Directive(LIMITMETHODS), _name("limit_methods") {
	/* this constructor takes one string which contains the limit methods that will be assigned. 
	   Ex:
			Methods("GET POST DELETE") [VALID]
			Methods("GET") [VALID]
			Methods("GIT") [throw InvalidDirectiveException]
			Methods("GET GET") [throw InvalidDirectiveException]
			Methods("GET POST NIHIL") [throw InvalidDirectiveException]
	*/
	char * str = std::strtok(const_cast<char*>(content.c_str()), " ");
	while (str) {
		if (!_validMethod(std::string(str)))
			throw InvalidDirectiveException();
		std::vector<std::string>::iterator last = _methods.end();
		std::vector<std::string>::iterator tmp = _methods.begin();
		for (; tmp != last; ++tmp)
			if (*tmp == str)
				throw InvalidDirectiveException(); // EVEN BETTER IF WE CREATE A "DUPLICATE EXCEPTION"
		_methods.push_back(std::string(str));
		str = std::strtok(NULL, " ");	
	}
	std::cout << WHITE << "Limit Methods created" << ENDC << std::endl;
}
Config::ServerConfig::Methods::~Methods() {
	std::cout << RED << "Limit Methods Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Listen::Listen(const std::string& content) throw (InvalidDirectiveException): Directive(LISTEN), _address("127.0.0.1"), _name("listen"), _port(80) {
	/*
	This constructor takes one string that contains the address and port of the server, separated by a colorn
	The constructor can take either the port only or the address only
	If one is not porvided the default settings are:
	//We should check if its just a port or address
	   Ex:
			Listen("SOME VALID EXAMPLE") [VALID]
			Methods("SOME INVALID EXAMPLE") [throw InvalidDirectiveException]
	*/
	(void)content;
}


/* Config Member Functions*/
bool	Config::validDirective(const std::string & str, const std::string * list, int len) const {
	int i(0);
	while (i < len)
		if (list[i++] == str)
			return (true);
	return (false);
}

void	Config::parseConfiguration() throw(InvalidDirectiveException) {
	short int context(0);
	std::string line;
    while (std::getline(_config_file, line)) {
		line = strtrim(line);
    	if (!line.length() || line[0] == '#')
    		continue;
    	std::string directive(line.substr(0, line.find_first_of(SEPARATORS)));
    	std::string tmp(line.substr(line.find_first_of(SEPARATORS) + 1, line.length()));
    	std::string directive_content(strtrim(tmp));
        if (directive == "}") {
        	if (--context < 0)
				throw e_invalid_directive;
			continue ;
        }
        std::cout << BLUE << std::left << std::setw(8) << (context == 2 ? "Location" : "Server") << " context: " << YELLOW;
        std::cout << "[" << directive << "]" << " - [" << directive_content << "]";
        switch (context) {
        	case 0:
        		if (directive == "server" && directive_content == "{")
        			context++;
        		else
			 		throw e_invalid_directive;
        		break;
			case 1:
		        if (validDirective(directive, _server_directives, SERVER_CONTEXT_DIRECTIVES))
		        {
		        	// CREATE SERVER DIRECTIVE
		        	std::cout << GREEN << "[OK]";
					if (directive == "location")
		        		context++;
		        }
		        else
		        {
		        	std::cout << RED << "[INVALID DIRECTIVE]" << ENDC;
					throw e_invalid_directive;
		        }
				break;
			case 2:
				if (validDirective(directive, _location_directives, LOCATION_CONTEXT_DIRECTIVES))
				{
					// CREATE LOCATION DIRECTIVE
		        	std::cout << GREEN << "[OK]";
				}
		        else {
					std::cout << RED << "[INVALID DIRECTIVE]" << ENDC;
					throw e_invalid_directive;
				}
				break;
			default:
				std::cout << ENDC;
				throw e_invalid_directive;
        }
        std::cout << std::endl;
    }
}
/* ServerConfig Member Functions */
void Config::ServerConfig::setRoot(const Root & root) {
	_root = root.getPath();
}

std::string & Config::ServerConfig::getRoot() {
	return _root;
}

void Config::ServerConfig::setMethods(const Methods& methods) {
	_methods = methods.getMethods();
}

std::vector<std::string>& Config::ServerConfig::getMethods() {
	return _methods;
}

/* ServerConfig Private Member Functions */
// TODO

/* Directives Member Functions */

int Config::ServerConfig::Directive::getId() const {
	return (_id);
}

const std::string &	Config::ServerConfig::Root::getName() const {
	return (_name);
}

const std::string & Config::ServerConfig::Root::getPath() const{
	return _path;
}

bool Config::ServerConfig::Root::_validPath(const std::string& path) {
	std::string::const_iterator itr = path.begin();
	for (; itr != path.end(); itr++) {
		if (isspace(*itr))
			return false; 
	}
	return true;
}

std::vector<std::string> Config::ServerConfig::Methods::getMethods() const {
	return _methods;
}

const std::string& Config::ServerConfig::Methods::getName() const {
	return _name;
}

bool Config::ServerConfig::Methods::_validMethod(const std::string& method) {
	for(size_t i = 0; i < 3; i++) {
		if (method == _valid_methods[i])
			return true;
	}
	return false;
}

const std::string& Config::ServerConfig::Listen::getName() const {
	return _name;
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
    if (brackets_parse % 2 != 0)
    	throw e_wrong_syntax;
    std::cout << "Status: All brackets closed" << std::endl;
    return ;
}
/****************************/
