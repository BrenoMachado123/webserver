# include "CGIExecution.hpp"

static void push_back_env(std::vector<char *> & vec, std::string const & name, std::string const & value) {
	if (!name.empty() && !value.empty()) {
		std::string tmp(value);
		tmp = name + "=" + strtrim(tmp);
		if (tmp.length() < 8000)
			vec.push_back(strdup(tmp.c_str()));
	}
}

CGIExecution::CGIExecution(const Request& req, const std::string& ip, int port) :
_request(req), _env(), _arg(), _fds(), _ip(ip), _port(port) {
    _populateEnviroment();
    _arg.push_back(strdup(_request.getCGIFile().c_str()));
	_arg.push_back(NULL);
}

CGIExecution::~CGIExecution() {}

int CGIExecution::run(std::string& content) {
	FILE* tmp_file_in = tmpfile();
	FILE* tmp_file_out = tmpfile();
    if (!_defineFds(tmp_file_in, tmp_file_out)) {
        return (-1);
	}
	write(_fds["TMP_IN"], _request.getContent().c_str(), _request.getContent().length());
	rewind(tmp_file_in);
	int pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0) {
		dup2(_fds["TMP_IN"], STDIN_FILENO);
		dup2(_fds["TMP_OUT"], STDOUT_FILENO);
		execve(_request.getCGIFile().c_str(), &_arg[0], &_env[0]);
		exit(EXIT_FAILURE);
    }
    int child_status;
	waitpid(pid, &child_status, 0);
    close(_fds["TMP_IN"]);
    rewind(tmp_file_out);
    if (child_status != 0){
    	content = "ERROR!!!";
    	return (500);
    }
    else if (!_transferFdToContent(_fds["TMP_OUT"], content)) { 
        return (-1);
    }
    close(_fds["TMP_OUT"]);
	_restoreFds();
	std::cout << YELLOW << " CGI Child Finished with status: " << ENDC << child_status;
	if (CONSTRUCTORS_DESTRUCTORS_DEBUG)
		std::cout << std::endl << "Content" << std::endl << content << std::endl;
	return (200);
}

void CGIExecution::_populateEnviroment() {
    std::stringstream ss;
    ss << _request.getContent().length();
	push_back_env(_env, "PATH", _request.getCGIFile());
	//push_back_env(env, "AUTH_TYPE", "");
	push_back_env(_env, "CONTENT_LENGTH", ss.str());
	push_back_env(_env, "CONTENT_TYPE", "application/x-www-form-urlencoded"); //+ _request.getContentType());
	push_back_env(_env, "DOCUMENT_ROOT", _request.getCGIBinPath());
	push_back_env(_env, "GATEWAY_INTERFACE", "CGI/1.1");
	push_back_env(_env, "HTTP_ACCEPT", "application/x-www-form-urlencoded,text/xml,application/xml,application/xhtml+xml,text/html,text/plain,charset=utf-8;");
	//push_back_env(env, "HTTP_COOKIE", "");
	//push_back_env(env, "HTTP_PRAGMA", "");
	push_back_env(_env, "HTTP_USER_AGENT", _request.getUserAgent());
	push_back_env(_env, "PATH_INFO", _request.getCGIBinPath());
	push_back_env(_env, "PATH_TRANSLATED", _request.getCGIBinPath());
	push_back_env(_env, "QUERY_STRING", _request.getQuery());
	//push_back_env(env, "REMOTE_ADDR", "");
	push_back_env(_env, "REMOTE_HOST", _ip);
	//push_back_env(env, "REMOTE_IDENT", "");
	//push_back_env(env, "REMOTE_PORT", "");
	//push_back_env(env, "REMOTE_USER", "");
	push_back_env(_env, "REQUEST_METHOD", _request.getMethod());
	push_back_env(_env, "REQUEST_URI", _request.getUriTarget());
	push_back_env(_env, "SCRIPT_FILENAME", _request.getCGIFile());
	push_back_env(_env, "SCRIPT_NAME", _request.getCGIFile());
	push_back_env(_env, "SERVER_ADMIN", "pulgamecanica11@gmail.com");
	push_back_env(_env, "SERVER_NAME", "BRTOAN");
    ss.str(std::string());
	ss << _port;
	push_back_env(_env, "SERVER_PORT", ss.str());
	push_back_env(_env, "SERVER_PROTOCOL", "HTTP/1.1");
	push_back_env(_env, "SERVER_SOFTWARE", "Webserv42.0 (Linux)");
	_env.push_back(NULL);
}

void CGIExecution::_restoreFds() {
	dup2(_fds["IN"], STDIN_FILENO);
	dup2(_fds["OUT"], STDOUT_FILENO);
	close(_fds["IN"]);
	close(_fds["OUT"]);
}

bool CGIExecution::_defineFds(FILE*& tmp_file_in, FILE*& tmp_file_out) {
    _fds["IN"] = dup(STDIN_FILENO);
	_fds["OUT"] = dup(STDOUT_FILENO);
	if (!tmp_file_in || !tmp_file_out || _fds["IN"] < 0)
		return (false);
    _fds["TMP_IN"] = fileno(tmp_file_in);
    _fds["TMP_OUT"] = fileno(tmp_file_out);
	if (_fds["TMP_IN"] < 0)
		return (false);
    return (true);
}

bool CGIExecution::_transferFdToContent(int fd, std::string& target) {
  char buff[1024];
  int valread = -1;
  while(valread != 0) {
      bzero(buff, 1024);
    valread = read(fd, buff, 1024);
    if (valread < 0)
      return (false);
    target += buff;
  }
  return (true);
}