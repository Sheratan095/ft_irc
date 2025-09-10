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

	client->setImBot(true);
}