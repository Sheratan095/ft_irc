#include "Utils.hpp"

int	main(int argc, char **argv)
{
	int	port = parse_input(argc, argv);

	Server	server = Server(port, argv[2]);
	server.start_server();
}
