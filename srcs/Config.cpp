#include "Config.hpp"

/* Initialize static class members */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "listen", "server_name", "error_page", "client_max_body_size", "location"};
const std::string Config::_location_directives[LOCATION_CONTEXT_DIRECTIVES] = {"root", "index", "limit_methods", "autoindex", "error_page"};
const std::string Config::ServerConfig::Methods::_valid_methods[3] = {"GET", "POST", "DELETE"};
const int Config::ServerConfig::ErrorCodePage::_allErrorCodes[ALL_ERROR_CODES] = {
	400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
	410, 411, 412, 413, 414, 415, 416, 417, 418,
	421, 422, 423, 424, 425, 426, 428, 429, 431, 451,
	500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511
};
/* Exceptions */
const char * Config::InvalidConfigurationFileException::what() const throw() {return ("Invalid File, make sure you have permissions, that the file exists and the extension is .conf");}
const char * Config::InvalidDirectiveException::what() const throw() {return ("Directive is invalid");}
const char * Config::WrongSyntaxException::what() const throw() {return ("Wrong Directive Syntax");}

// CONSTUCTORS & DESTRUCTORS //
Config::Config(std::ifstream & file) throw(InvalidConfigurationFileException): _config_file(file) {
	if (!file.is_open())
		throw e_invalid_configuration_file;
	std::cout << WHITE << "Config created" << ENDC << std::endl;
	parseConfiguration();
	/*{
		// TESTING 
		// ServerConfig::ErrorCodePage ep1("401 nice.txt");
		// ServerConfig::ErrorCodePage ep2("401 505 yes.txt");
		// ServerConfig::ErrorCodePage ep3("401 400 401 uppps.txt");// FAILING THIS TEST
	}
    {
        try {
            ServerConfig::Listen l("122.22.22.0:8040");
            ServerConfig::Listen l1("*:8040");
            ServerConfig::Listen l2("0.0.0.0:10");
            ServerConfig::Listen l3("localhost");
            ServerConfig::Listen l4("localhost:80");
            std::cout << YELLOW << "Port: " << l.getPort() << ", Address: " <<  l.getIp() << ENDC << std::endl;
            std::cout << YELLOW << "Port: " << l1.getPort() << ", Address: " <<  l1.getIp() << ENDC << std::endl;
            std::cout << YELLOW << "Port: " << l2.getPort() << ", Address: " <<  l2.getIp() << ENDC << std::endl;
            std::cout << YELLOW << "Port: " << l3.getPort() << ", Address: " <<  l3.getIp() << ENDC << std::endl;
            std::cout << YELLOW << "Port: " << l4.getPort() << ", Address: " <<  l4.getIp() << ENDC << std::endl;
        } catch (std::exception & e) {
           std::cout << BLUE << e.what() << std::endl;
        }
    }*/
}

Config::~Config() {
	std::cout << RED << "Config" << " destroyed" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig(): _address("127.0.0.1"), _root("html/"), _port(80) {
	std::cout << WHITE << "ServerConfig created" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig(const ServerConfig &copy) {
    *this = copy;
}

Config::ServerConfig &Config::ServerConfig::operator=(const ServerConfig &other) {
    if (this == &other)
        return *this;
    this->_address = other._address;
    this->_root = other._root;
    this->_port = other._port;
    this->_name = other._name;
    this->_errorCodes = other._errorCodes;
    this->_errorPath = other._errorPath;
    this->_ip = other._ip;
    this->_methods = other._methods;
    return *this;
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

Config::ServerConfig::Root::Root(const std::string & str) throw (InvalidDirectiveException):
	Directive(ROOT), _path(str), _name("root") {
	/*
	This constructor takes only one string which should be a valid path
	Ex:
		Root("/etc/www/root") [VALID]
		Root("/etc trash wrong") [throw InvalidDirectiveException]
		Root("") [throw InvalidDirectiveException]
	*/
	if (str.empty() || !_validPath(str))
		throw InvalidDirectiveException();
	std::cout << WHITE << "Root created" << ENDC << std::endl;
}

Config::ServerConfig::Root::~Root() {
	std::cout << RED << "Root Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Methods::Methods(const std::string& content) throw (InvalidDirectiveException):
	Directive(LIMITMETHODS), _name("limit_methods") {
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
    std::cout << RED << "Methods Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::ErrorCodePage::ErrorCodePage(const std::string & content) throw(InvalidDirectiveException):
    Directive(ERRORPAGE), _name("errorCodePage") {
    /*
    This constructor takes only one string which should be valid error code/s
    and the URI (page to display in case given error event occures)
    Ex:
        ErrorCodePage("404 405 ./response.html")    [VALID]
        ErrorCodePage("500 ./500.html")             [VALID]
        ErrorCodePage("404")                        [throw InvalidDirectiveException]
        ErrorCodePage("404 405")                    [throw InvalidDirectiveException]
        ErrorCodePage("404 405 ./r.html ./el.html") [throw InvalidDirectiveException]
        ErrorCodePage("404, 405 ./response.html")   [throw InvalidDirectiveException]
        ErrorCodePage("./response.html")            [throw InvalidDirectiveException]
        ErrorCodePage("./response.html 404")        [throw InvalidDirectiveException]
        ErrorCodePage(100 ./100.html")              [throw InvalidDirectiveException]
    */
    int found = content.find_last_of(SEPARATORS);
    if (content.empty()|| found == -1
        || !isCodeValid(content.substr(0, found)))
        throw InvalidDirectiveException();
    _errorPath = content.substr(found + 1);
    std::cout << WHITE <<"ErrorCode Created" << std::endl;
}

Config::ServerConfig::ErrorCodePage::~ErrorCodePage() {
    std::cout << RED << "ErrorPage Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Listen::Listen(const std::string &content)
    throw (InvalidDirectiveException):
    Directive(LISTEN), _name("listen"), _ip("127.0.0.1"), _port(80) {
     /*
    This constructor takes only one string which can be the ip_address and port separated by a color or either without any colon
    Ex:
        Listen("*:80")          [VALID]
        Listen("0.0.0.0:80")    [VALID]
        Listen("124.10.20.30")  [VALID]
        Listen("localhost:80")  [VALID]
        Listen("4242")          [VALID]
        Listen("500000")        [throw InvalidDirectiveException]
        Listen("42 42")         [throw InvalidDirectiveException]
        Listen("0.0.0.2")       [throw InvalidDirectiveException]
        Listen("127,0.4.5")     [throw InvalidDirectiveException]
    */

    if (content.empty() || content.find_first_of(SEPARATORS) != std::string::npos)
        throw InvalidDirectiveException();
    std::string temp;
    std::stringstream stoi_converter;
    if (content.find(':') != std::string::npos) {
        std::string ip_str = content.substr(0, content.find(':'));
        std::string port_str = content.substr(content.find(':') + 1);
        if (!isIpValid(ip_str) || port_str.empty())
                throw InvalidDirectiveException();
        else if (ip_str == "*")
            _ip = "0.0.0.0";
        else if (ip_str == "localhost")
            _ip = "127.0.0.1";
        else
            _ip = ip_str;
        temp = port_str;
    }
    else if (isIpValid(content)) {
        temp = content;
        _port = 80;
        if (!temp.compare("localhost"))
            _ip = "127.0.0.1";
        else if (!temp.compare("*"))
            _ip = "0.0.0.0";
        else
            _ip = temp;
    }
    else {
        stoi_converter << content;
        stoi_converter >> _port;
        if (_port > PORT_MAX || _port <= PORT_MIN)
            throw InvalidDirectiveException();
    }
    //IT IS ALL GOOD HERE
    //std::cout << "IP: " << _ip << "PORT" << _port;
    std::cout << WHITE << "Listen created" << ENDC << std::endl;
}

Config::ServerConfig::Listen::~Listen() {
    std::cout << RED << "Listen destroyed!" << ENDC << std::endl;
}

/* Config Member Functions*/

bool Config::validDirective(const std::string & str, const std::string * list, int len) const {
	int i(0);
	while (i < len)
		if (list[i++] == str)
			return (true);
	return (false);
}

void Config::createDirective(const std::string &dir, const std::string &cont) {
    if (dir == "listen") {
        std::cout << "\nCREATING LISTEN" << std::endl;
        Config::ServerConfig::Listen listen(cont);
        //_servers.back().getDirective().push_back(listen);
        //Config::ServerConfig::Listen *l = new Config::ServerConfig::Listen(cont);
        _servers.back().setListen(listen);
        std::cout << _servers.back().getListenPort() << ": PORT | IP: " << _servers.back().getListenIp() << std::endl;
        //std::cout << l->getPort() << ": PORT | IP: " << l->getIp() << std::endl;
        std::cout << "IS LISTEN DESTRYED?" << std::endl;
    }
    /*else if (dir == "server_name")
        Config::ServerConfig::ServerName serverName(cont);
    else if (dir == "error_page")
        Config::ServerConfig::ErrorCodePage errorCodePage(cont);
    else if (dir == "root")
        Config::ServerConfig::Root root(cont);
    else if (dir == "client_max_body_size")
        Config::ServerConfig::ClientMBS clientMBS(cont);
    else if (dir == "location")
        Config::ServerConfig::Location location(cont);
    else if (dir == "index")
        Config::ServerConfig::Index index(cont);
    else if (dir == "limit_methods")
        Config::ServerConfig::limit_methods limitMethods(cont);
    else if (dir == "autoindex")
        Config::ServerConfig::autoindex autoindex(cont);*/
}

void Config::parseConfiguration() throw(InvalidDirectiveException) {
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
        		std::cout << std::endl;
        		if (directive == "server" && directive_content == "{") {
                    // create a server object and push it to the vector
                    Config::ServerConfig s;
                    std::cout << RED << "ERROR IS HERE?" << std::endl;
                    _servers.push_back(s);
        			context++;
        		}
        		else
			 		throw e_invalid_directive;
			 	continue ;
			case 1:
		        if (validDirective(directive, _server_directives, SERVER_CONTEXT_DIRECTIVES))
		        {
		        	// TODO CREATE SERVER DIRECTIV
		        	std::cout << GREEN << "[OK]";
					if (directive == "location")
                    {
		        		context++;
                        /*
                        //servers.locations.addLocation(content)
                        */
                    }
                    else {
                        createDirective(directive, directive_content);
                        //_servers.back().getDirective().at(LISTEN);
                    }
                    /*
                    //servers.last().push_back(directive)
                    */
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
					// TODO CREATE LOCATION DIRECTIVE
                    /*
                    // servers.last().locations.last().addDirective(directive)
                    */
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

/* Config Member Functions */
std::vector<Config::ServerConfig> Config::getServer() {
    return _servers;
}

std::vector<std::string> Config::getResponse() {
    return _responses;
}

/* ServerConfig Member Functions */
void Config::ServerConfig::setRoot(const Root & root) {_root = root.getPath();}
std::string & Config::ServerConfig::getRoot() {return _root;}
void Config::ServerConfig::setMethods(const Methods& methods) {_methods = methods.getMethods();}
std::vector<std::string>& Config::ServerConfig::getMethods() {return _methods;}
void Config::ServerConfig::setErrorCodePage(const ErrorCodePage &errorCodePage) {
    _errorCodes = errorCodePage.getErrorCodes();
    _errorPath = errorCodePage.getErrorPath();
}
std::vector<int> &Config::ServerConfig::getErrorCodes() {return _errorCodes;}
std::string &Config::ServerConfig::getErrorPath() {return _errorPath;}
void Config::ServerConfig::setListen(const Listen &listen) {
    _port = listen.getPort();
    _ip = listen.getIp();
}
int &Config::ServerConfig::getListenPort() {
    return _port;
}
std::string &Config::ServerConfig::getListenIp() {return _ip;}

//std::vector<Config::ServerConfig::Directive> Config::ServerConfig::getDirective() {
//    return _directives;
//}

//std::vector<Config::ServerConfig::Directive> Config::ServerConfig::findDirective(int dirnum) {
//    std::vector<Config::ServerConfig::Directive>::iterator it = _directives.begin();
//    for (; it != _directives.end() ; it++)
//        if (dirnum == it->getId())
//            return _directives.at(*it);
//    return ;
//}

/* Directives Member Functions */

int Config::ServerConfig::Directive::getId() const {return (_id);}

const std::string &	Config::ServerConfig::Root::getName() const {return (_name);}

const std::string & Config::ServerConfig::Root::getPath() const{return _path;}


// SHould change this.... (remove) 
// TODO
bool Config::ServerConfig::Root::_validPath(const std::string &path) {
    std::string::const_iterator itr = path.begin();
    for (; itr != path.end(); itr++) {
        if (isspace(*itr))
            return false;
    }
    return true;
}

const std::string& Config::ServerConfig::Methods::getName() const {return _name;}

std::vector<std::string> Config::ServerConfig::Methods::getMethods() const {return _methods;}

bool Config::ServerConfig::Methods::_validMethod(const std::string& method) {
    for(size_t i = 0; i < 3; i++) {
        if (method == _valid_methods[i])
            return true;
    }
    return false;
}

const std::string &Config::ServerConfig::ErrorCodePage::getName() const {return _name;}

const std::string &Config::ServerConfig::ErrorCodePage::getErrorPath() const {return _errorPath;}

const std::vector<int> &Config::ServerConfig::ErrorCodePage::getErrorCodes() const {return _errorCodes;}

bool Config::ServerConfig::ErrorCodePage::isCodeValid(const std::string &content) {
    std::stringstream stoi_converter;
    int converted_number;
    int loop_counter = 0;
    char *token = std::strtok(const_cast<char*>(content.c_str()), SEPARATORS);
    while (token != 0) {
        loop_counter++;
        stoi_converter.clear();
        stoi_converter << token;
        stoi_converter >> converted_number;
        for (int i = 0; i < 40; i++) {
            if (converted_number == _allErrorCodes[i]) {
                _errorCodes.push_back(converted_number);
                loop_counter--;
            }
        }
        token = std::strtok(NULL, SEPARATORS);
    }
    if (!loop_counter)
        return true;
    return false;
}

const std::string &Config::ServerConfig::Listen::getName() const {return _name;}

const std::string &Config::ServerConfig::Listen::getIp() const {return _ip;}

int Config::ServerConfig::Listen::getPort() const {return _port;}

bool Config::ServerConfig::Listen::isIpValid(const std::string &ip) {
    if (!ip.compare("0.0.0.0") || !ip.compare("localhost") || !ip.compare("*"))
        return true;
//Verifies if number of dots in IP is = 3;
    int counter = 0;
    for (u_int16_t i = 0 ; i <= ip.length() ; i++)
        if (ip[i] == '.')
            counter++;
    if (counter != 3)
        return false;

    std::stringstream stoi_converter(ip);
    int part1, rest;
    char ch;
    stoi_converter >> part1 >> ch;
    if (part1 < 1 || part1 > 255 || ch != '.')
        return false;
    while (stoi_converter.rdbuf()->in_avail() != 0) {
        stoi_converter >> rest >> ch;
        if (rest < 0 || rest > 255)
            return false;
        if (ch != '.')
            return false;
    }
    return true;
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
    return;
}
/****************************/
