#include "Irc.hpp"

void	handleSigInt(int sig)
{
	if (sig	==	SIGINT)
	{
		std::cout << "\nInterrupt signal received. Shutting down server..." << std::endl;
		SERVER_RUNNING = false;
	}
}

int	parseInput(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "ERROR: Usage: " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}

	std::string	port = argv[1];
	std::string	psw = argv[2];

	if (port.size() == 0 || psw.size() == 0)
	{
		std::cerr << "ERROR: Usage: " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}

	// Find position of a character not in C string
	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		std::cerr << "ERROR: Invalid port number" << std::endl;
		exit(1);
	}

	double	portnm = std::strtod(port.c_str(), NULL);
	if (portnm < 0 || portnm > 65535)
	{
		std::cerr << "ERROR:  Invalid port number" << std::endl;
		exit(1);
	}

	return (portnm);
}
