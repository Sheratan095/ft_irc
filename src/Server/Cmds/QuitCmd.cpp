#include "Server.hpp"

void	Server::quitCmd(Client *client, const IRCMessage &message)
{
	if (!client)
		return;

	if (message.parameters.size() > 0)
		notifyQuit(client, message.trailing);
	else
		notifyQuit(client, "");

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

void	Server::quitCmd(Client *client, const std::string &reason)
{
	if (!client)
		return;

	if (reason.empty())
		notifyQuit(client, "");
	else
		notifyQuit(client, reason);

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
