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


void	Server::removeClient(Client *client)
{
	// Remove the client from every channels that he's connected to
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(client->getSocketFd()))
			it->second->removeClient(client->getSocketFd());
	}

	// Remove quitting client from other clients' sets
	for (std::map<SocketFd, std::set<SocketFd> >::iterator it = _privateConversations.begin(); it != _privateConversations.end(); ++it)
		it->second.erase(client->getSocketFd());

	// Remove quitting client’s own set
	_privateConversations.erase(client->getSocketFd());

	int	client_fd = client->getSocketFd();
	std::cout << "Client with fd: " << client_fd << " disconnected" << std::endl << std::endl;

	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == client_fd)
		{
			_pollFds.erase(it);
			break;
		}
	}

	delete (client); // Free the memory allocated for the client
	_clients.erase(client_fd); // Remove the client from the map
}