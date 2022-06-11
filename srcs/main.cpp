#include "/home/przemek/42/webserver/includes/webserv.hpp"
//#include "Epoll.hpp"
static void help(char *prog_name) {
	std::cout << RED << "usage " << prog_name << " config_file.conf" << std::endl;
	std::cout << YELLOW << "hints: check if configuration file exists." << ENDC << std::endl;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		help(av[0]);
		return (1);
	}
	std::ifstream file;
	file.open(av[1], std::ios::in);
    long int bytes;
    Config conf(file);
    Server ser;
    Epoll _epoll;
    //conf.getResponse().push_back("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    std::string hello = "HTTP/1.1 200 OK\\nContent-Type: text/plain\\nContent-Length: 12\\n\\nHello world!";
    /*try {
		Config conf(file);
	} catch (std::exception & e)
 	{
		std::cout << e.what() << std::endl;
	}*/

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
        _epoll.epollWait();
        for (int n = 0 ; n < _epoll.getnfds() ; n++) {
            int pos = whatServer(_epoll, n);
            if (pos != -1) {
                _epoll.getServerSocket().at(pos).accept();
                _epoll.epollControl(EPOLL_CTL_ADD, _epoll.getServerSocket().at(pos).getNewConnection());
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

	return (0);
}
