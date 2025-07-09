#include "Server.hpp"

int	getPort(std::string strPort);

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}

	Server	server = Server(getPort(argv[1]), argv[2]);
	server.start_server();
}

int	getPort(std::string strPort)
{
	try
	{
		// Parses the C string str interpreting its content as a floating point number
		int	port = std::strtold(strPort.c_str(), NULL);
		return (port);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return (-1);
}