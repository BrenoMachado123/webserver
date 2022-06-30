#include "Config.hpp"
#include "utils.hpp"

/* STATIC CLASS MEMBERS */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "listen", "server_name", "error_page", "client_max_body_size", "location", "index", "autoindex"};
const std::string Config::_location_directives[LOCATION_CONTEXT_DIRECTIVES] = {"root", "index", "limit_methods", "autoindex", "error_page", "client_max_body_size"};
const std::string Config::ServerConfig::Methods::_valid_methods[3] = {"GET", "POST", "DELETE"};
const int Config::ServerConfig::ErrorCodePage::_allErrorCodes[ALL_ERROR_CODES] = {
	400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
	410, 411, 412, 413, 414, 415, 416, 417, 418,
	421, 422, 423, 424, 425, 426, 428, 429, 431, 451,
	500, 501, 502, 503, 504, 505, 506, 507, 508, 510, 511
};
/* EXCEPTIONS */
const char * Config::InvalidConfigurationFileException::what() const throw() {return ("Invalid File, make sure you have permissions, that the file exists and the extension is .conf");}
const char * Config::InvalidDirectiveException::what() const throw() {return ("Directive is invalid");}
const char * Config::WrongSyntaxException::what() const throw() {return ("Wrong Directive Syntax");}
/* CONSTUCTORS & DESTRUCTORS */
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
                break ;
            case 1:
                if (validDirective(directive, _server_directives, SERVER_CONTEXT_DIRECTIVES))
                {
                    std::cout << GREEN << "[OK]" << ENDC << std::endl;
                    ServerConfig::Directive * _directive;
                    _directive = createDirective(directive, directive_content);
                    if (_directive != 0)
                    {
                        _directive->setDirective(_servers.back(), context);
                        if (directive == "location")
                            context++;
                        delete (_directive);
                    }
                    else
                        throw e_wrong_syntax;
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
                    std::cout << GREEN << "[OK]" << ENDC << std::endl;
                    ServerConfig::Directive * _directive;
                    _directive = createDirective(directive, directive_content);
                    if (_directive != 0)
                    {
                        _directive->setDirective(_servers.back(), context);
                        delete (_directive);
                    }
                    else
                        throw e_wrong_syntax;
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
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Config created" << ENDC << std::endl;
}
Config::~Config() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Config destroyed" << ENDC << std::endl;
}

Config::ServerConfig::ServerConfig() : _autoindex(false), _max_body_size(-1), _ip("127.0.0.1"), _port(80), _root_path("www"), _error_path("www/errors") {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "ServerConfig created [" << _ip << ":" << _port << "]" << ENDC << std::endl;
}
// Config::ServerConfig::ServerConfig(const ServerConfig & serv_conf):
//     _ip(serv_conf._ip), _port(serv_conf._port) {
//     if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
//         std::cout << WHITE << "Copy ServerConfig created [" << _ip << ":" << _port << "]" << ENDC << std::endl;
// }
Config::ServerConfig::~ServerConfig() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "ServerConfig destroyed" << ENDC << std::endl;
}

Config::ServerConfig::Directive::Directive(int id): _id(id) {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Directive(" << id << ") created" << ENDC << std::endl;
}
Config::ServerConfig::Directive::~Directive() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Directive destroyed" << ENDC << std::endl;
}

Config::ServerConfig::AutoIndex::AutoIndex(const std::string & content) throw (InvalidDirectiveException):
    Directive(AUTOINDEX) {
    /* Configure webserv to return an automatically generated directory listing instead,
    include the on parameter to the autoindex directive:.*/
    if (content == "on")
        _option = true;
    else if (content == "off")
        _option = false;
    else
        throw InvalidDirectiveException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "AutoIndex created [" << (_option == true ? "on" : "off") << "]" << ENDC << std::endl;
}
Config::ServerConfig::AutoIndex::~AutoIndex() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "AutoIndex Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::ClientMaxBodySize::ClientMaxBodySize(const std::string & content) throw (InvalidDirectiveException):
    Directive(SERVERNAME), _max_size(0) {
    /*Sets the maximum allowed size of the client request body. 
    If the size in a request exceeds the configured value, 
    the 413 (Request Entity Too Large) error is returned to the client.
    Please be aware that browsers cannot correctly display this error. 
    Setting size to 0 disables checking of client request body size.*/
    if (content.find(SEPARATORS) != std::string::npos)
        throw InvalidDirectiveException();
    std::stringstream intValue(content);
    intValue >> _max_size;
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "ClientMaxBodySize created!" << ENDC << std::endl;
}
Config::ServerConfig::ClientMaxBodySize::~ClientMaxBodySize() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ClientMaxBodySize Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::ErrorCodePage::ErrorCodePage(const std::string & content) throw(InvalidDirectiveException):
    Directive(ERRORPAGE) {
    /* This constructor takes only one string which should be valid error code/s
    and the URI (page to display in case given error event occures)
        ErrorCodePage("404 405 ./response.html")    [VALID]
        ErrorCodePage("500 ./500.html")             [VALID]
        ErrorCodePage("404 405")                    [throw InvalidDirectiveException]
        ErrorCodePage("404 405 ./r.html ./el.html") [throw InvalidDirectiveException]
        ErrorCodePage("./response.html")            [throw InvalidDirectiveException] */
    size_t found = content.find_last_of(SEPARATORS);
    if (content.empty()|| found == std::string::npos
        || !isCodeValid(content.substr(0, found)))
        throw InvalidDirectiveException();
    _error_path = content.substr(found + 1);
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE <<"ErrorCode created" << std::endl;
}
Config::ServerConfig::ErrorCodePage::~ErrorCodePage() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ErrorPage Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Index::Index(const std::string & content) throw (InvalidDirectiveException):
    Directive(INDEX) {
    /*You can list more than one filename in the index directive.
    Webserv searches for files in the specified order and returns the first one it finds.*/
    if (content.empty())
        throw InvalidDirectiveException();
    std::string tmp(content);
    char *token = strtok(const_cast<char*>(tmp.c_str()), SEPARATORS);
    while (token != NULL) {
        _indexes.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Index created!" << ENDC << std::endl;
}
Config::ServerConfig::Index::~Index() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Index Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Methods::Methods(const std::string & content) throw (InvalidDirectiveException):
	Directive(LIMITMETHODS) {
	/* This constructor takes one string which contains the limit methods that will be assigned. 
		Methods("GET POST DELETE") [VALID]
		Methods("GET")             [VALID]
		Methods("GET GET")         [throw InvalidDirectiveException]
		Methods("GET POST NIHIL")  [throw InvalidDirectiveException] */
	char * str = std::strtok(const_cast<char*>(content.c_str()), " ");
	while (str) {
		if (!_validMethod(std::string(str)))
            throw InvalidDirectiveException();
		std::vector<std::string>::iterator last = _methods.end();
		std::vector<std::string>::iterator tmp = _methods.begin();
		for (; tmp != last; ++tmp)
			if (*tmp == str)
                throw InvalidDirectiveException();
		_methods.push_back(std::string(str));
		str = std::strtok(NULL, " ");	
	}
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Limit Methods created" << ENDC << std::endl;
}
Config::ServerConfig::Methods::~Methods() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Methods destroyed" << ENDC << std::endl;
}

Config::ServerConfig::Listen::Listen(const std::string & content) throw (InvalidDirectiveException):
    Directive(LISTEN), _ip("127.0.0.1"), _port(80) {
    /* This constructor takes only one string which can be the ip_address and port separated by a color or either without any colon
        Listen("*:80")          [VALID]
        Listen("124.10.20.30")  [VALID]
        Listen("localhost:80")  [VALID]
        Listen("4242")          [VALID]
        Listen("500000")        [throw InvalidDirectiveException]
        Listen("42 42")         [throw InvalidDirectiveException]
        Listen("127,0.4.5")     [throw InvalidDirectiveException] */
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
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Listen created" << ENDC << std::endl;
}
Config::ServerConfig::Listen::~Listen() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Listen destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Location::Location(std::string const & content) throw (InvalidDirectiveException):
    Directive(LOCATION), _error_path("www/default_error/"), _target(content), _max_body_size(-1), _autoindex(false) {
    /*This constructor takes only one string which should be a valid path
        Location("/etc/www/where_is_the_file {") [VALID]
        Location("/etc trash wrong {") [throw InvalidDirectiveException]
        Location("") [throw InvalidDirectiveException] */
    if (content.empty() || content[content.length() - 1] != '{')
        throw InvalidDirectiveException();
    _target = _target.substr(0, content.length() - 1);
    _target = strtrim(_target);
    if (_target.empty() || _target.find(SEPARATORS) != std::string::npos)
        throw InvalidDirectiveException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "Location created" << ENDC << std::endl;
}
Config::ServerConfig::Location::~Location() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "Location Directive destroyed!" << ENDC << std::endl;
}

Config::ServerConfig::Root::Root(const std::string & str) throw (InvalidDirectiveException):
	Directive(ROOT), _path(str) {
	/* This constructor takes only one string which should be a valid path
		Root("/etc/www/root")    [VALID]
		Root("/etc trash wrong") [throw InvalidDirectiveException]
		Root("")                 [throw InvalidDirectiveException] */
	if (str.empty() || str.find(SEPARATORS) != std::string::npos)
		throw InvalidDirectiveException();
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << WHITE << "Root created" << ENDC << std::endl;
}
Config::ServerConfig::Root::~Root() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
	   std::cout << RED << "Root destroyed" << ENDC << std::endl;
}

Config::ServerConfig::ServerName::ServerName(const std::string & content) throw (InvalidDirectiveException):
    Directive(SERVERNAME) {
    /*First, a connection is created in a default server context.
    Then, the server name can be determined in the following request processing stage:
    after processing the request line, after processing the Host header field.
    If the server name was not determined after processing the request line or from the Host header field,
    will use the empty name as the server name.*/
    std::string tmp(content);
    char *token = strtok(const_cast<char*>(tmp.c_str()), SEPARATORS);
    while (token != NULL) {
        _server_names.push_back(token);
        token = strtok(NULL, SEPARATORS);
    }
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << WHITE << "ServerName created!" << ENDC << std::endl;
}
Config::ServerConfig::ServerName::~ServerName() {
    if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
        std::cout << RED << "ServerName Directive destroyed!" << ENDC << std::endl;
}

/* MEMBER FUNCTIONS */
bool Config::validDirective(const std::string & str, const std::string * list, int len) const {
	int i(0);
	while (i < len)
		if (list[i++] == str)
			return (true);
	return (false);
}

Config::ServerConfig::Directive * Config::createDirective(std::string const & name, std::string const & content) throw(std::exception) {

    if (name == "listen") {
        return (new ServerConfig::Listen(content));
    }
    else if (name == "error_page") {
        return (new ServerConfig::ErrorCodePage(content));
    }
    else if (name == "root") {
        return (new ServerConfig::Root(content));
    }
    else if (name == "limit_methods") {
        return (new ServerConfig::Methods(content));
    }
    else if (name == "location") {
        return (new ServerConfig::Location(content));
    }
    else if (name == "server_name") {
        return (new ServerConfig::ServerName(content));
    }
    else if (name == "client_max_body_size")  {
        return (new ServerConfig::ClientMaxBodySize(content));
    }
    else if (name == "index") {
        return (new ServerConfig::Index(content));
    }
    else if (name == "autoindex") {
        return (new ServerConfig::AutoIndex(content));
    }
    return (0);
}

int Config::ServerConfig::getPort() const {return _port;}

std::string const & Config::ServerConfig::getIp() const {return _ip;}

Config::ServerConfig::Location * Config::ServerConfig::findLocation(std::string const & target) const {
    size_t matches(0);
    std::vector<Location> tmp_locs(_locations);
    std::vector<Location>::iterator l_it;
    std::vector<Location>::iterator tmp_it;
    std::cout << "Searching..." << std::endl;
    for (l_it = tmp_locs.begin(); l_it != tmp_locs.end(); ++l_it) {
        int tmp = target.compare(0, l_it->_target.length(), l_it->_target);
        if (tmp == 0 && l_it->_target.length() > matches) {
            matches = l_it->_target.length();
            tmp_it = l_it;
            std::cout << GREEN << std::endl << "Matches " << l_it->_target << ENDC << std::endl;
        }
        else
            std::cout << RED << std::endl << "Doesn't Matches " << l_it->_target << ENDC << std::endl;
    }
    if (matches == 0)
        return (0);
    return (new Location(*tmp_it));
}

// std::string target_location(0, _target.find_last_of("/"));
// target_location += "/";

int Config::ServerConfig::Directive::getId() const {return (_id);}

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
                _error_codes.push_back(converted_number);
                loop_counter--;
            }
        }
        token = std::strtok(NULL, SEPARATORS);
    }
    if (!loop_counter)
        return true;
    return false;
}

bool Config::ServerConfig::Methods::_validMethod(const std::string& method) {
    for(size_t i = 0; i < 3; i++) {
        if (method == _valid_methods[i])
            return true;
    }
    return false;
}

bool Config::ServerConfig::findMethod(const std::string& method) const {
    for(size_t i = 0; i < 3; i++) {
        if (method == Methods::_valid_methods[i])
            return true;
    }
    return false;
}

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

void Config::ServerConfig::AutoIndex::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._autoindex = _option;
    else if (context == 2)
        serv_conf._locations.back()._autoindex = _option;
}

void Config::ServerConfig::ClientMaxBodySize::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._max_body_size = _max_size;
    else if (context == 2)
        serv_conf._locations.back()._max_body_size = _max_size;
}

void Config::ServerConfig::ErrorCodePage::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._error_path = _error_path;
    else if (context == 2)
        serv_conf._locations.back()._error_path = _error_path;
}

void Config::ServerConfig::Index::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._indexes = _indexes;
    else if (context == 2)
        serv_conf._locations.back()._indexes = _indexes;
}

void Config::ServerConfig::Methods::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 2)
        serv_conf._locations.back()._methods = _methods;
}

void Config::ServerConfig::Listen::setDirective(ServerConfig & serv_conf, int context) const {
    if (context != 1)
        return ;
    serv_conf._port = _port;
    serv_conf._ip = _ip;
}

void Config::ServerConfig::Location::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._locations.push_back(*this);
}

void Config::ServerConfig::Root::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._root_path = _path;
    else if (context == 2)
        serv_conf._locations.back()._root_path = _path;
}

void Config::ServerConfig::ServerName::setDirective(ServerConfig & serv_conf, int context) const {
    if (context == 1)
        serv_conf._names = _server_names;
}

// std::string const &Config::ServerConfig::Location::getLocation() const{return _location;}
// bool &Config::ServerConfig::Location::l_getAutoindex() {return _autoindex;}
// std::string Config::ServerConfig::Location::l_getRoot() const {return _l_root;}
// std::vector<int> &Config::ServerConfig::Location::l_getErrorCodes() {return _l_errorCodes;}
// std::string &Config::ServerConfig::Location::l_getErrorPath() {return _l_errorPath;}
// std::vector<std::string> &Config::ServerConfig::Location::l_getMethods() {return _l_methods;}

/* CLASS STATIC FUNCTIONS */

std::ostream& operator<<(std::ostream & s, const Config & param) {
	s << "Some configuration Text";
	(void)param;
	return (s);
}

std::ostream& operator<<(std::ostream & s, const Config::ServerConfig & param) {
    s << "|**********************************************|"<< std::endl;
    s << "| Server " << param.getIp() << ":" << param.getPort() << std::endl;
    s << "| - root path " << param._root_path << std::endl;
    s << "| - error path " << param._error_path << std::endl;
    s << "| - server names [";
    std::vector<std::string> s_list(param._names);
    std::vector<std::string>::iterator s_it(s_list.begin());
    for (; s_it != s_list.end() ; ++s_it)
        s << *s_it << " ";
    s << "]" << std::endl;
    s << "| - autoindex " << (param._autoindex == true ? "on" : "off") << std::endl;
    s << "| - max_body_size " << param._max_body_size << std::endl;
    s << "| - [" << param._locations.size() << "] locations " << std::endl;
    std::vector<Config::ServerConfig::Location> list = param._locations;
    std::vector<Config::ServerConfig::Location>::iterator it(list.begin());
    for (; it != list.end() ; ++it) {
        s << "|  + target " << it->_target << std::endl;
        s << "|     root path " << it->_root_path << std::endl;
        s << "|     error path " << it->_error_path << std::endl;
        s << "|     autoindex " << (it->_autoindex == true ? "on" : "off") << std::endl;
        s << "|     max_body_size " << it->_max_body_size << std::endl;
        s << "|     Index [";
        std::vector<std::string>::iterator i_it(it->_indexes.begin());
        for (; i_it != it->_indexes.end() ; ++i_it)
            s << *i_it << " ";
        s << "]" << std::endl;
        s << "|     Methods [";
        std::vector<std::string>::iterator m_it(it->_methods.begin());
        for (; m_it != it->_methods.end() ; ++m_it)
            s << *m_it << " ";
        s << "]" << std::endl;
    }
    s << "|**********************************************|"<< std::endl;
    return (s);
}
