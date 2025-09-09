#include "Server.hpp"

void	Server::listCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "INVITE");
		return;
	}

	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "LIST");
		return;
	}
}