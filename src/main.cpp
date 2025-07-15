#include "Irc.hpp"

int	main(int argc, char **argv)
{
	int	port = parseInput(argc, argv);

	Server	server = Server(port, argv[2]);
	try
	{
		server.startServer();
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}
}
