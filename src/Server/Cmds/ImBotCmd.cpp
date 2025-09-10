#include "Server.hpp"

// A client can set itself as an instant messaging bot
void	Server::imBotCmd(Client *client, const IRCMessage &message)
{
	(void)message;

	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "IMBOT");
		return;
	}

	if (message.trailing.empty())
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "IMBOT");
		return;
	}

	client->setUsage(message.trailing);
	client->setImBot(true);

	// Notify all the other registered clients about the new bot
	for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->isRegistered() == true && it->second != client)
			sendMessage(it->second->getSocketFd(), client->getNickname() + " bot is connected to the server. Usage: " + client->getUsage() + "\r\n");
	}
}