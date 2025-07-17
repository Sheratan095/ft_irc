#include "Server.hpp"

bool	Server::addClient(pollfd clientPollFd)
{
	// Check if the client already exists
	if (_clients.find(clientPollFd.fd) != _clients.end())
	{
		std::cerr << "Client with fd " << clientPollFd.fd << " already exists." << std::endl;
		return (false);
	}

	if (_clients.size() >= MAX_CLIENTS)
	{
		std::cerr << "Maximum number of clients reached. Cannot add new client." << std::endl;
		return (false);
	}

	// Create a new Client object and add it to the map
	Client*	newClient = new Client(clientPollFd.fd);
	_clients[clientPollFd.fd] = newClient;

	// Add the client pollfd to the poll list
	_pollFds.push_back(clientPollFd);

	return (true);
}
