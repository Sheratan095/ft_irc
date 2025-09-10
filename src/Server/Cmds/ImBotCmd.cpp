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
}