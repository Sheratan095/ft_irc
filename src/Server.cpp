#include "Server.hpp"

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

bool	Server::start_server()
{
	// std::cout << "Server is listening on port " << port << " and the password is '" << password << "'" << std::endl;

	// int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	// // Allow quick reuse of the port
	// int opt = 1;
	// setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// // Bind to port
	// struct sockaddr_in	addr;
	// memset(&addr, 0, sizeof(addr));
	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// addr.sin_port = htons(port);
	// bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));

	// // Start listening
	// listen(socket_fd, SOMAXCONN);

	return (true);
}