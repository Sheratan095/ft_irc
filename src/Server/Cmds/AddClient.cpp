#include "Server.hpp"

// Used JUST for new connections
bool	Server::addClient(pollfd clientPollFd, const std::string &ip_str)
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
	Client*	newClient = new Client(clientPollFd.fd, ip_str);
	_clients[clientPollFd.fd] = newClient;

	// Add the client pollfd to the poll list
	_pollFds.push_back(clientPollFd);

	return (true);
}
