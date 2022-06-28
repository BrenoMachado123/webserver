#include "webserv.hpp"
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

	try {
		HTTPServer s(av[1]);
		// {
		// 	Socket s1("127.0.0.1", 4242);
		// 	Socket s2("127.0.0.1", 4243);
		// 	Socket s3("127.0.0.1", 4244);
		// 	s.addSocket(s1);
		// 	s.addSocket(s2);
		// 	s.addSocket(s3);
		// }
		s.run();
	} catch (std::exception & e) {
		std::cout << RED << "EXCEPTION: " << e.what() << std::endl;
	}
	
	std::cout << GREEN << "FINISHED" << ENDC << std::endl;

	return (0);
}
