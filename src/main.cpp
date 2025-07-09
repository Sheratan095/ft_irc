#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server	server = Server(6667, "password123");
	server.start_server();
}
