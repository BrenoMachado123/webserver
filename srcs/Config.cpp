#include "Config.hpp"

/* Initialize static class members */
const std::string Config::_server_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "lsiten"};
const std::string Config::_listen_directives[SERVER_CONTEXT_DIRECTIVES] = {"root", "index"};
const int Config::ServerConfig::ErrorCodePage::_allErrorCodes[ALL_ERROR_CODES] =
        {400, 401, 402, 403, 404, 405, 406, 407, 408, 409,
          410, 411, 412, 413, 414, 415, 416, 417, 418,
          421, 422, 423, 424, 425, 426, 428, 429,
          431,
          451,
          500, 501, 502, 503, 504, 505, 506, 507, 508,
          510, 511};
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
        ServerConfig::Listen l1("*:8040");
        ServerConfig::Listen l2("0.0.0.0:10");
//        ServerConfig::Listen l3("localhost");
        ServerConfig::Listen l4("localhost:80");
        std::cout << YELLOW << "Port: " << l.getPort() << ", Address: " <<  l.getIp() << ENDC << std::endl;
        std::cout << YELLOW << "Port: " << l1.getPort() << ", Address: " <<  l1.getIp() << ENDC << std::endl;
        std::cout << YELLOW << "Port: " << l2.getPort() << ", Address: " <<  l2.getIp() << ENDC << std::endl;
//        std::cout << YELLOW << "Port: " << l3.getPort() << ", Address: " <<  l3.getIp() << ENDC << std::endl;
        std::cout << YELLOW << "Port: " << l4.getPort() << ", Address: " <<  l4.getIp() << ENDC << std::endl;
	} catch (InvalidDirectiveException & e)
	{
		std::cout << RED << e.what() << std::endl;
	}

    /* ERROR CODE PAGE */
    try {
        ServerConfig::ErrorCodePage ecp("404 505 ./response.html");
        std::vector<int>::const_iterator it = ecp.getErrorCodes().begin();
        std::cout << YELLOW << "Error Code(s): ";
        for (; it != ecp.getErrorCodes().end() ; it++)
            std::cout << *it << " ";
        std::cout << ", Path: " << ecp.getErrorPath() << ENDC << std::endl;
    } catch (InvalidDirectiveException &e) {
        std::cout  << RED << e.what() << std::endl;
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

Config::ServerConfig::Root::Root(const std::string & str) throw (InvalidDirectiveException):
    Directive(ROOT), _path(str), _name("root") {
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

//===========================ERROR_PAGES===============================//
Config::ServerConfig::ErrorCodePage::ErrorCodePage(const std::string & content) throw(InvalidDirectiveException):
    Directive(ERRORPAGE), _name("errorCodePage") {
    /*
    This constructor takes only one string which should be valid error code/s
    and URI (page to display in case given error event occures)
    Ex:
        valid_inputs{"404 405 ./response.html",
                        "500 ./500.html"};

        invalid_inputs{"404",
                        "404 405",
                        "404 405 ./response.html ./elo.html"
                        "404, 405 ./response.html",
                        "399 ./399.html",
                        "100 ./100.html",
                        "./response.html",
                        "./response.html 404"
    }; throw InvalidDirectiveExeption
    */
    //possible implementation:

    int converted_number = 0;
    std::stringstream stoi_converter;
    char *token = std::strtok(const_cast<char*>(content.c_str()), " \t\n\v\f\r");
    while (token != 0) {
        if (*token >= 48 && *token <= 57) {
            stoi_converter.clear();
            stoi_converter << token;
            stoi_converter >> converted_number;
            if (!isCodeValid(converted_number))
                throw InvalidDirectiveException();
            _errorCodes.push_back(converted_number);
        }
        else {
            if (!isStringValid(token))
                throw InvalidDirectiveException();
            if (!_errorCodes.empty() && _errorPath.empty()) {
                _errorPath = token;
            }
            else
                throw InvalidDirectiveException();
        }
        token = std::strtok(NULL, " \t\n\v\f\r");
    }
    if (_errorPath.empty())
        throw InvalidDirectiveException();
    std::cout << WHITE <<"ErrorCodePage directive Created" << std::endl;
}

Config::ServerConfig::ErrorCodePage::~ErrorCodePage() {
    std::cout << RED << "ErrorPage directive Destroyed" << ENDC << std::endl;
}


//=============================LISTEN==================================//

    Config::ServerConfig::Listen::Listen(
    const std::string &content) throw (InvalidDirectiveException):
    Directive(LISTEN), _name("listen"), _ip("127.0.0.1"), _port(80) {
        /*
        This constructor takes only one string which should be a valid path
        Ex:
            valid_inputs{"*:80", "0.0.0.0:80", "0.0.0.0", "124.10.20.30",
                            "125.10.20.40:190", "localhost:80", "127.12.0.1.",
                            "4242"};
            invalid_inputs{"500000", "42 42",
                            "0.0.0.2", "127.256.0.1", "127,0.4.5", "126:256.0.1", "127:256.0.1:80"};
                [throw InvalidDirectiveException]
        */
        //possible implementation:

        if (!isStringValid(content))
            throw InvalidDirectiveException();
        int test_port;
        std::string temp;
        std::stringstream stoi_converter;
        if (content.find(':') != std::string::npos) {
            char *token = std::strtok(const_cast<char *>(content.c_str()), ":");
            if (*token == '*' || !static_cast<std::string>(token).compare("0.0.0.0"))
                _ip = "0.0.0.0";
            else if (!static_cast<std::string>(token).compare("localhost"))
                _ip = "127.0.0.1";
            else {
                if (!isIpValid(token))
                    throw InvalidDirectiveException();
                _ip = token;
            }
            if ((token = std::strtok(NULL, " ")) == 0) {
                _port = 80;
                exit(EXIT_SUCCESS);
            }
            temp = token;
        } else {
            temp = content;
            if (isIpValid(temp)) {
                _port = 80;
                _ip = temp;
                exit(EXIT_SUCCESS);
            }
            if (temp.find('.') != std::string::npos)
                throw InvalidDirectiveException();
        }
        stoi_converter << temp;
        stoi_converter >> test_port;
        if (test_port > PORT_MAX || test_port <= PORT_MIN)
            throw InvalidDirectiveException();
        _port = test_port;
        std::cout << WHITE << "Listen created" << ENDC << std::endl;
    }

    Config::ServerConfig::Listen::~Listen()
    {
        std::cout << RED << "Listen Directive destroyed" << ENDC << std::endl;
    }

    Config::ServerConfig::Methods::Methods(
    const std::string &content) throw (InvalidDirectiveException):
    Directive(LIMITMETHODS), _name("limit_methods")
    {
        /* this constructor takes one string which contains the limit methods
           that will be assigned.

           Ex:
                Methods("GET POST DELETE") [VALID]
                Methods("GET") [VALID]
                Methods("GIT") [throw InvalidDirectiveException]
                Methods("GET POST NIHIL") [throw InvalidDirectiveException]
        */
        // possible implemetation:
        char *str = std::strtok(const_cast<char *>(content.c_str()), " ");
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

    Config::ServerConfig::Methods::~Methods()
    {
        std::cout << RED << "Limit Methods Directive destroyed!" << ENDC << std::endl;
    }






/* ServerConfig Member Functions */

    void Config::ServerConfig::setRoot(const Root &root) {
        _root = root.getPath();
    }

    std::string &Config::ServerConfig::getRoot() {
        return _root;
    }

    void Config::ServerConfig::setErrorCodePage(const ErrorCodePage &errorCodePage) {
        _errorCodes = errorCodePage.getErrorCodes();
        _errorPath = errorCodePage.getErrorPath();
    }

    std::vector<int> &Config::ServerConfig::getErrorCodes() {
        return _errorCodes;
    }

    std::string &Config::ServerConfig::getErrorPath() {
        return _errorPath;
    }

    void Config::ServerConfig::setListen(const Listen &listen) {
        _port = listen.getPort();
        _ip = listen.getIp();
    }

    int &Config::ServerConfig::getListenPort() {
        return _port;
    }

    std::string &Config::ServerConfig::getListenIp() {
        return _ip;
    }

    void Config::ServerConfig::setMethods(const Methods &methods) {
        _methods = methods.getMethods();
    }

    std::vector<std::string> &Config::ServerConfig::getMethods() {
        return _methods;
    }





/* ServerConfig Private Member Functions */

    bool Config::ServerConfig::Methods::_validMethod(const std::string &method) {
        std::string valid_methods[3] = {"GET", "POST", "DELETE"};
        for (size_t i = 0; i < 3; i++) {
            if (method == valid_methods[i])
                return true;
        }
        return false;
    }

    bool Config::ServerConfig::Root::_validPath(const std::string &path) {
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

    const std::string &Config::ServerConfig::Root::getName() const {
        return (_name);
    }

    const std::string &Config::ServerConfig::Root::getPath() const {
        return _path;
    }


    const std::string &Config::ServerConfig::ErrorCodePage::getName() const {
        return _name;
    }

    const std::string &Config::ServerConfig::ErrorCodePage::getErrorPath() const {
        return _errorPath;
    }

    const std::vector<int> &Config::ServerConfig::ErrorCodePage::getErrorCodes() const {
        return _errorCodes;
    }

    bool Config::ServerConfig::ErrorCodePage::isCodeValid(const int &code) {
        for (int i = 0 ; i < ALL_ERROR_CODES ; i++)
            if (code == _allErrorCodes[i])
                return true;
        return false;
    }

    bool Config::ServerConfig::ErrorCodePage::isStringValid(const std::string &path) {
        std::string::const_iterator it = path.begin();
        for (; it != path.end() ; it++)
            if (isspace(*it))
                return false;
        return true;
    }


    const std::string &Config::ServerConfig::Listen::getName() const {
        return _name;
    }

    const std::string &Config::ServerConfig::Listen::getIp() const {
        return _ip;
    }

    const int &Config::ServerConfig::Listen::getPort() const {
        return _port;
    }

    bool Config::ServerConfig::Listen::isStringValid(const std::string &content) {
        std::string::const_iterator it = content.begin();
        for (; it != content.end(); ++it)
            if (isspace(*it))
                return false;
        return true;
    }

    bool Config::ServerConfig::Listen::isIpValid(const std::string &ip) {
        if (!ip.compare("0.0.0.0"))
            return true;
//Verifies if number of dots in IP is = 3;
        int counter = 0;
        for (u_int16_t i = 0 ; i <= ip.length() ; i++)
            if (ip[i] == '.')
                counter++;
        if (counter > 3)
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

    std::vector<std::string> Config::ServerConfig::Methods::getMethods() const {
        return _methods;
    }

    const std::string &Config::ServerConfig::Methods::getName() const {
        return _name;
    }


/* HELPER FUNCTIONS */










    std::ostream &operator<<(std::ostream &s, const Config &param) {
        s << "Some configuration Text";
        (void) param;
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
