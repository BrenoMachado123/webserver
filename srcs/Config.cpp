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
	/* ROOT TEST */
	std::cout << YELLOW << "Server Root Configuration: " << s.getRoot() << ENDC << std::endl;
	ServerConfig::Root r("/some_valid/path");
	/* METHODS TEST */
	ServerConfig::Methods m("GET POST");
	std::vector<std::string> methods_list(m.getMethods());
	std::vector<std::string>::iterator e = methods_list.end();
	for(std::vector<std::string>::iterator b = methods_list.begin(); b != e; ++b)
		std::cout << YELLOW << *b << " ";
	std::cout << ENDC << std::endl;
	/* LISTEN TEST */
	try {
		ServerConfig::Listen l("122.22.22.0:8040");
		std::cout << YELLOW << "Port: " << l.getPort() << ", Address: " <<  l.getIp() << ENDC << std::endl;
	} catch (InvalidDirectiveException & e)
	{
		std::cout << RED << e.what() << std::endl;
	}

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

Config::ServerConfig::ServerConfig(): _address("127.0.0.1"), _root("html/"), _port(80) {
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
	//possible implementation:
	if (!str.size() or !_validPath(str))
		throw InvalidDirectiveException();
	std::cout << WHITE << "Root created" << ENDC << std::endl;
}

Config::ServerConfig::Root::~Root() {
	std::cout << RED << "Root Directive destroyed!" << ENDC << std::endl;
}

//===========================LISTEN===============================//

Config::ServerConfig::Listen::Listen(const std::string &content) throw(InvalidDirectiveException): Directive(LISTEN), _name("listen"), _ip("127.0.0.1"), _port(80) {
    /*
	This constructor takes only one string which should be a valid path
	Ex:
		Listen("/4242") [VALID]
		Listen("42 42") [throw InvalidDirectiveException]
		Listen("123090") [throw InvalidDirectiveException]
        Listen("12") [throw InvalidDirectiveException]
		Listen("") [throw InvalidDirectiveException]
	*/
    //possible implementation:
    if (!isValid(content))
        throw InvalidDirectiveException();
    std::string temp;
    std::stringstream stoi_converter;
//Checking if port was provided as address:port or just port
    if (content.find(':') != std::string::npos) {
        char *cstr = new char[content.length() + 1];
        std::strcpy(cstr, content.c_str());
        char *tokens = std::strtok(cstr, ":");
        _ip = *tokens;
        temp = *tokens + 1;
        delete[] cstr;
    } else
        temp = content;
    stoi_converter << temp;
    stoi_converter >> _port;
    if (_port > PORT_MAX || _port <= PORT_MIN)
        throw InvalidDirectiveException();
	std::cout << WHITE << "Listen created" << ENDC << std::endl;
}

Config::ServerConfig::Listen::~Listen() {
    std::cout << RED << "Listen Directive destroyed" << ENDC << std::endl;
}

Config::ServerConfig::Methods::Methods(const std::string& content) throw (InvalidDirectiveException):
    Directive(LIMITMETHODS), _name("limit_methods") {
	/* this constructor takes one string which contains the limit methods
	   that will be assigned. 

	   Ex:
			Methods("GET POST DELETE") [VALID]
			Methods("GET") [VALID]
			Methods("GIT") [throw InvalidDirectiveException]
			Methods("GET POST NIHIL") [throw InvalidDirectiveException]
	*/
	// possible implemetation:
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

/* ServerConfig Member Functions */

void Config::ServerConfig::setRoot(const Root & root) {
	_root = root.getPath();
}

std::string & Config::ServerConfig::getRoot() {
	return _root;
}

void Config::ServerConfig::setListen(const Listen & listen) {
    _port = listen.getPort();
    _ip = listen.getIp();
}

int &Config::ServerConfig::getListenPort() {
    return _port;
}

std::string &Config::ServerConfig::getListenIp() {
    return _ip;
}

void Config::ServerConfig::setMethods(const Methods& methods) {
	_methods = methods.getMethods();
}

std::vector<std::string>& Config::ServerConfig::getMethods() {
	return _methods;
}

/* ServerConfig Private Member Functions */

bool Config::ServerConfig::Listen::isValid(const std::string & content) {
    std::string::const_iterator it = content.begin();
    for (; it != content.end(); ++it)
        if (isspace(*it))
            return false;
    return true;
}


bool Config::ServerConfig::Methods::_validMethod(const std::string& method) {
	std::string valid_methods[3] = {"GET", "POST", "DELETE"};
	for(size_t i = 0; i < 3; i++) {
		if (method == valid_methods[i])
			return true;
	}
	return false;
}

bool Config::ServerConfig::Root::_validPath(const std::string& path) {
	std::string::const_iterator itr = path.begin();
	for (; itr != path.end(); itr++) {
		if (isspace(*itr))
			return false; 
	}
	return true;
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

const std::string &Config::ServerConfig::Listen::getName() const{
    return _name;
}

const std::string &Config::ServerConfig::Listen::getIp() const {
    return _ip;
}

const int &Config::ServerConfig::Listen::getPort() const {
    return _port;
}

std::vector<std::string> Config::ServerConfig::Methods::getMethods() const {
	return _methods;
}

const std::string& Config::ServerConfig::Methods::getName() const {
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
    _config_file.close();
    if (brackets_parse % 2 != 0)
    	throw e_wrong_syntax;
    std::cout << "Status: All brackets closed" << std::endl;
    return ;
}

/****************************/
