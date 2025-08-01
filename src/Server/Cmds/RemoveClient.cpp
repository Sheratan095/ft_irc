#include "Server.hpp"

void	Server::removeClient(Client *client)
{
	if (!client)
		return;

	int	client_fd = client->getSocketFd();
	std::cout << "Client with fd: " << client_fd << " disconnected" << std::endl;

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
