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
	std::cout << "Server is listening on port " << port << " and the password is '" << password << "'" << std::endl;
	// Here you would implement the logic to start the server.
	// For now, we just return true to indicate success.

	return (true);
}