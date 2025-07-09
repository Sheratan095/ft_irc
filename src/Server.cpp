#include "Server.hpp"

Server::Server(const unsigned short port, const std::string &password):port(port), password(password)
{
	if (password.empty())
		throw std::invalid_argument("Password cannot be empty.");


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