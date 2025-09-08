#include "Server.hpp"

// Used when a real cmd is sent
void	Server::quitCmd(Client *client, const IRCMessage &message)
{
	if (!client)
		return;

	// Check for quit message
	if (message.trailing.empty() == false)
		notifyQuit(client, message.trailing);
	else
		notifyQuit(client, "Client disconnected");

	removeClient(client);
}

// Used in other situations, like problems or unespected quit
void	Server::quitCmd(Client *client, const std::string &reason)
{
	if (!client)
		return;
	
	if (reason.empty())
		notifyQuit(client, reason);
	else
		notifyQuit(client, "Client disconnected");

	client->getSocketFd();

	removeClient(client);
}
