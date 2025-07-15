#include "Server.hpp"

bool SERVER_RUNNING = true;

Server::Server(const int port, const std::string &password): port(port), password(password)
{
	if (password.empty())
		throw std::invalid_argument("Password cannot be empty.");

	// 0 - 1024 are well-known ports and are used for standard services.
	// 1024 - 49151 are registered ports and can be used by user applications
	// 49152 - 65535 are dynamic or private ports and can be used for ephemeral(effimera) connections.
	if (port < 1024 || port > 65535)
		throw std::out_of_range("Port must be between 1024 and 65535.");

	std::cout << "Server created with port: " << port << " and password: '" << password << "'" << std::endl;
}

Server::~Server()
{
	std::cout << "Server destroyed." << std::endl;
}

//----------------------------------------------------------

void	Server::startServer()
{
	signal(SIGINT, handleSigInt);

	if (!createSocket())
		throw (SocketException());

	if (!bindSocket())
		throw (BindException());

	if (!startListening())
		throw (ListenException());

	std::cout << "Server is listening on port " << port << std::endl;

	// Set socket timeout to allow periodic checking of SERVER_RUNNING
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	while (SERVER_RUNNING)
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
		
		if (client_fd == -1)
		{
			if (!SERVER_RUNNING)
			{
				std::cout << "Server shutting down..." << std::endl;
				break;
			}
			continue;
		}

		handleClient(client_fd);
	}
	
	close(socket_fd);

}

bool	Server::createSocket()
{
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		// FAILED TO CREATE SOCKET
		return (false);
	}

	int	opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		// FAILED TO SET SOCKET OPTIONS
		close(socket_fd);
		return (false);
	}

	return (socket_fd);
}

bool	Server::bindSocket()
{
	struct sockaddr_in	addr;

	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		// FAILED TO BIND SOCKET
		close(socket_fd);
		return (false);
	}
	return (true);
}
