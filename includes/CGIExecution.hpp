#ifndef __CGIEXECUTION_HPP__
# define __CGIEXECUTION_HPP__

# include "Request.hpp"
# include <sys/wait.h>

/**
 * @brief Class that execute CGI
 * 
 */
class CGIExecution {
    public:
        CGIExecution(const Request&, const std::string&, int);
        ~CGIExecution();

        int run(std::string& content);
    
    private:
        Request const & _request;
        std::vector<char *> _env;
        std::vector<char *> _arg;
        std::map<std::string, int> _fds;
        std::string _ip;
        int _port;

        void _populateEnviroment(void);
        void _restoreFds(void);
        bool _defineFds(FILE*&, FILE*&);
        bool _transferFdToContent(int fd, std::string& target);
};

#endif // __CGIEXECUTION_HPP__
