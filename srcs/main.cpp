#include "webserv.hpp"
#include <fstream>
#include <sys/epoll.h>
#include <unistd.h>

static void help(char *prog_name) {
	std::cout << RED << "usage " << prog_name << " config_file.conf" << std::endl;
	std::cout << YELLOW << "hints: check if configuration file exists." << ENDC << std::endl;
}

int	main(int ac, char **av)
{
    long int bytes;
    Server ser;
    Epoll _epoll;
	std::ifstream file;

	if (ac != 2)
	{
		help(av[0]);
		return (1);
	}

    std::string hello = "HTTP/1.1 200 OK\\nContent-Type: text/plain\\nContent-Length: 12\\n\\nHello world!";
    try {
        file.open(av[1], std::ios::in);
		Config conf(file);

    // ************************* SERVER **************************//
        // assigning servers to the respective ports
        for (unsigned int i = 0 ; i < conf.getServer().size() ; i++) {
            ser.socket(AF_INET, SOCK_STREAM, 0);
            std::cout << "PORT: " << conf.getServer().at(i).getListenPort() << "IP: " << conf.getServer().at(i).getListenIp() << std::endl;
            ser.bind(conf.getServer().at(i).getListenPort(),
                     conf.getServer().at(i).getListenIp());
            ser.listen(3);
            _epoll.addSocket(ser);
        }
        _epoll.epollCreate(10);

        for (unsigned int i = 0 ; i < conf.getServer().size() ; i++) {
            _epoll.epollControl(EPOLL_CTL_ADD, _epoll.getServerSocket().at(i).getSocket());
        }

        for (;;) {
            std::cout << "Waiting for connection..." << std::endl << std::endl;
            std::cout << "Failing here: " << std::endl;
            _epoll.epollWait();
            for (int n = 0 ; n < _epoll.getnfds() ; n++) {
        //HERE WE CHECK IF CONNECTION COMES VIA ONE OF SERVERS FDS
        // This means we compare FD given by epoll (ready to read) with
        // list of server fds. If its the from this function we return the
        // position of the server fd's list. That position is later used to call
        //              _epoll.getServerSocket().at(pos).accept();
        // We call accept for that particular server object.
                int pos = whatServer(_epoll, n);
                if (pos != -1) {
        // Accept creates new fd (new_connection) and add that fd(new_connection) 
        // to the observing list.
                    _epoll.getServerSocket().at(pos).accept();
                    _epoll.epollControl(EPOLL_CTL_ADD, _epoll.getServerSocket().at(pos).getNewConnection());
                    std::cout << "PROBLEM1: " << std::endl << std::endl;
                }
                else {
                    char buffer[10000] = {0};
                    std::cout << "PROBLEM: " << std::endl << std::endl;
                    bytes = read(_epoll.getEpollEvent(n).data.fd, buffer, 10000);
                    if (bytes < 0) {
                        perror("READ: ");
                        throw;
                    }
                    std::cout << buffer << std::endl;
                    if (send(_epoll.getEpollEvent(n).data.fd,hello.c_str(),
                             hello.size(),0) < 0) {
                        perror("SEND: ");
                        throw;
                    }
                    //serverResponse(conf, _epoll, n);
                }
            }
        }
        //
    // ************************* SERVER **************************//

        file.close();
	} catch (std::exception & e)
 	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
