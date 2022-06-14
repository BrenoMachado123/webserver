#include "webserv.hpp"
#include "Socket.hpp"
#include "HTTPServer.hpp"

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
	std::cout << GREEN << "TESTING" << ENDC << std::endl;
	HTTPServer s;
	Socket s1("127.0.0.98", 4242);
	Socket s2("127.0.0.98", 8000);
	Socket s3("127.0.0.100", 4242);
	s.addSocket(s1);
	s.addSocket(s2);
	s.addSocket(s3);

	s.run();
	
	std::cout << GREEN << "FINISHED" << ENDC << std::endl;
	/*
	try {
		std::ifstream file;
		file.open(av[1], std::ios::in);
		Config c(file);
		file.close();
	} catch (std::exception & e)
 	{
		std::cout << e.what() << std::endl;
	}*/

	return (0);
}
