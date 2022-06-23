#include "Config.hpp"
#include "utils.hpp"

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
Config::Config(std::string const & file_str) throw(std::exception) {
    std::ifstream file;
    file.open(file_str.c_str(), std::ios::in);
	if (!file.is_open())
		throw e_invalid_configuration_file;
    short int context(0);
    std::string line;
    while (std::getline(file, line)) {
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
        std::cout << BLUE << std::left << (context == 2 ? "Location" : "Server") << " context: " << YELLOW;
        std::cout << "[" << directive << "]" << " - [" << directive_content << "]";
        switch (context) {
            case 0:
                std::cout << std::endl;
                if (directive == "server" && directive_content == "{") {
                    ServerConfig s;
                    _servers.push_back(s);
                    context++;
                }
                else
                    throw e_invalid_directive;
                continue ;
            case 1:
                if (validDirective(directive, _server_directives, SERVER_CONTEXT_DIRECTIVES))
                {
                    std::cout << GREEN << "[OK]" << std::endl;
                    ServerConfig::Directive * _directive;
                    _directive = createDirective(directive, directive_content);
                    if (_directive != 0)
                    {
                        _directive->setDirective(_servers.back());
                        if (_directive->getName() == "location")
                        {
                            context++;
                            /*
                            //servers.locations.addLocation(content)
                            */
                        }
                        delete (_directive);
                    }
                    if (directive== "location") // DELETE THIS DELETE THIS LATER
                        context++; // DELETE THIS DELETE THIS LATER
                    // else
                        //throw e_wrong_syntax;
                }
                else
                {
                    std::cout << RED << "[INVALID DIRECTIVE]" << ENDC << std::endl;
                    throw e_invalid_directive;
                }
                break;
            case 2:
                if (validDirective(directive, _location_directives, LOCATION_CONTEXT_DIRECTIVES))
                {
                    // TODO CREATE LOCATION DIRECTIVE
                    /*
                    // servers.back().locations.back().addDirective(directive)
                    */
                    std::cout << GREEN << "[OK]" << std::endl;
                }
                else {
                    std::cout << RED << "[INVALID DIRECTIVE]" << ENDC << std::endl;
                    throw e_invalid_directive;
                }
                break;
            default:
                std::cout << ENDC;
                throw e_invalid_directive;
        }
    }
	std::cout << WHITE << "Config created" << ENDC << std::endl;

    {
        // TESTING
        // ServerConfig::ErrorCodePage ep1("401 nice.txt");
        // ServerConfig::ErrorCodePage ep2("401 505 yes.txt");
        // ServerConfig::ErrorCodePage ep3("401 400 401 uppps.txt");// FAILING THIS TEST
        // try {
        //     ServerConfig::Listen l("122.22.22.0:8040");
        //     ServerConfig::Listen l1("*:8040");
        //     ServerConfig::Listen l2("0.0.0.0:10");
        //     ServerConfig::Listen l3("localhost");
        //     ServerConfig::Listen l4("localhost:80");
        //     std::cout << YELLOW << "Port: " << l.getPort() << ", Address: " <<  l.getIp() << ENDC << std::endl;
        //     std::cout << YELLOW << "Port: " << l1.getPort() << ", Address: " <<  l1.getIp() << ENDC << std::endl;
        //     std::cout << YELLOW << "Port: " << l2.getPort() << ", Address: " <<  l2.getIp() << ENDC << std::endl;
        //     std::cout << YELLOW << "Port: " << l3.getPort() << ", Address: " <<  l3.getIp() << ENDC << std::endl;
        //     std::cout << YELLOW << "Port: " << l4.getPort() << ", Address: " <<  l4.getIp() << ENDC << std::endl;
        // } catch (std::exception & e) {
        //    std::cout << BLUE << e.what() << std::endl;
        // }
    }
}

Config::~Config() {
	std::cout << RED << "Config" << " destroyed" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig() { // : _address("127.0.0.1"), _root("html/"), _port(80) {
	std::cout << WHITE << "ServerConfig created" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig(const ServerConfig & serv_conf) {
    _port = serv_conf._port;
    _ip = serv_conf._ip;
    std::cout << WHITE << "Copy ServerConfig created" << ENDC << std::endl;
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
		Root("/etc/www/root")    [VALID]
		Root("/etc trash wrong") [throw InvalidDirectiveException]
		Root("")                 [throw InvalidDirectiveException]
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
	/* 
    This constructor takes one string which contains the limit methods that will be assigned. 
    Ex:
		Methods("GET POST DELETE") [VALID]
		Methods("GET")             [VALID]
		Methods("GIT")             [throw InvalidDirectiveException]
		Methods("GET GET")         [throw InvalidDirectiveException]
		Methods("GET POST NIHIL")  [throw InvalidDirectiveException]
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
    std::cout << WHITE << "Listen created" << ENDC << std::endl;
}

Config::ServerConfig::Listen::~Listen() {
    std::cout << RED << "Listen destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Location::Location(std::string const &content) throw (InvalidDirectiveException):
        Directive(LOCATION), _location(content), _name("location"), _autoindex(false) {

    /*
	This constructor takes only one string which should be a valid path
	Ex:
		Location("/etc/www/where_is_the_file") [VALID]
		Location("/etc trash wrong") [throw InvalidDirectiveException]
		Location("") [throw InvalidDirectiveException]
	*/
    if (content.empty() || content.find_first_of(SEPARATORS) != std::string::npos) {
        throw InvalidDirectiveException();
    }
    std::cout << WHITE << "Location created" << ENDC << std::endl;
}

Config::ServerConfig::Location::~Location() {
    std::cout << RED << "Location Directive destroyed!" << ENDC << std::endl;
}

/* Config Member Functions*/

bool Config::validDirective(const std::string & str, const std::string * list, int len) const {
	int i(0);
	while (i < len)
		if (list[i++] == str)
			return (true);
	return (false);
}

Config::ServerConfig::Directive * Config::createDirective(std::string const & name, std::string const & content) throw(std::exception) {
    if (name == "listen")
    {
        return (new ServerConfig::Listen(content));
    }
    return (0);
}


/* ServerConfig Member Functions */
// void Config::ServerConfig::setRoot(const Root & root) {_root = root.getPath();}
// std::string & Config::ServerConfig::getRoot() {return _root;}
// void Config::ServerConfig::setMethods(const Methods& methods) {_methods = methods.getMethods();}
// std::vector<std::string>& Config::ServerConfig::getMethods() {return _methods;}
// void Config::ServerConfig::setErrorCodePage(const ErrorCodePage &errorCodePage) {
//     _errorCodes = errorCodePage.getErrorCodes();
//     _errorPath = errorCodePage.getErrorPath();
// }
// std::vector<int> &Config::ServerConfig::getErrorCodes() {return _errorCodes;}
// std::string &Config::ServerConfig::getErrorPath() {return _errorPath;}


int Config::ServerConfig::getPort() const {return _port;}
std::string const & Config::ServerConfig::getIp() const {return _ip;}

void Config::ServerConfig::setLocation(const Location &loc) {
    _location = loc.getLocation();
}

std::string & Config::ServerConfig::getLocation() {
    return _location;
}

std::vector<Config::ServerConfig::Location> & Config::ServerConfig::getLocations() {
    return _locations;
}

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

void Config::ServerConfig::Root::setDirective(ServerConfig & serv_conf) const {(void)serv_conf;}

const std::string& Config::ServerConfig::Methods::getName() const {return _name;}

std::vector<std::string> Config::ServerConfig::Methods::getMethods() const {return _methods;}

bool Config::ServerConfig::Methods::_validMethod(const std::string& method) {
    for(size_t i = 0; i < 3; i++) {
        if (method == _valid_methods[i])
            return true;
    }
    return false;
}

void Config::ServerConfig::Methods::setDirective(ServerConfig & serv_conf) const {(void)serv_conf;}

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

void Config::ServerConfig::ErrorCodePage::setDirective(ServerConfig & serv_conf) const {(void)serv_conf;}

const std::string &Config::ServerConfig::Listen::getName() const {return _name;}

const std::string &Config::ServerConfig::Listen::getIp() const {return _ip;}

int Config::ServerConfig::Listen::getPort() const {return _port;}

bool Config::ServerConfig::Listen::isIpValid(const std::string &ip) {
    if (!ip.compare("0.0.0.0") || !ip.compare("localhost") || !ip.compare("*"))
        return true;
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


void Config::ServerConfig::Listen::setDirective(ServerConfig & serv_conf) const {
    serv_conf._port = _port;
    serv_conf._ip = _ip;
}

void Config::ServerConfig::Location::setDirective(ServerConfig & serv_conf) const {
    serv_conf._location = _location;
}

std::string const &Config::ServerConfig::Location::getName() const {
    return _name;
}

std::string const &Config::ServerConfig::Location::getLocation() const{
    return _location;
}

/* LOCATION MEMBER FUNCTIONS */

void Config::ServerConfig::Location::l_setAutoindex(bool value) {
    _autoindex = value;
}

bool &Config::ServerConfig::Location::l_getAutoindex() {
    return _autoindex;
}

void Config::ServerConfig::Location::l_setRoot(const Root & root) {
    _l_root = root.getPath();
}

std::string Config::ServerConfig::Location::l_getRoot() const {
    return _l_root;
}

void Config::ServerConfig::Location::l_setErrorCodePage(const ErrorCodePage &er) {
    _l_errorCodes = er.getErrorCodes();
    _l_errorPath = er.getErrorPath();
}

std::vector<int> &Config::ServerConfig::Location::l_getErrorCodes() {
    return _l_errorCodes;
}

std::string &Config::ServerConfig::Location::l_getErrorPath() {
    return _l_errorPath;
}

void Config::ServerConfig::Location::l_setMethods(const Methods &met) {
    _l_methods = met.getMethods();
}

std::vector<std::string> &Config::ServerConfig::Location::l_getMethods() {
    return _l_methods;
}

/* HELPER FUNCTIONS */

std::ostream& operator<<(std::ostream & s, const Config & param) {
	s << "Some configuration Text";
	(void)param;
	return (s);
}

std::ostream& operator<<(std::ostream & s, const Config::ServerConfig & param) {
    s << "Server " << param.getIp() << ":" << param.getPort();
    return (s);
}
