#include "webserv.hpp"

static void help(char *prog_name) {
	std::cout << RED << "usage ./" << prog_name << " config_file.conf" << std::endl;
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
	Config c(file);
	file.close();

	return (0);
}
