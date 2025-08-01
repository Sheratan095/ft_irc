
#include "Server.hpp"

void	Server::removeClient(int client_fd)
{
	std::cout << "Client with fd: " << client_fd << " disconnected" << std::endl;

	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == client_fd)
		{
			_pollFds.erase(it);
			break;
		}
	}


	if (_clients.find(client_fd) != _clients.end())
	{
		delete (_clients[client_fd]); // Free the memory allocated for the client

		_clients.erase(client_fd); // Remove the client from the map
	}
}
