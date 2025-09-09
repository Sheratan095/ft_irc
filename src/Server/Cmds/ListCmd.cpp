#include "Server.hpp"

// FORMAT: /LIST
void	Server::listCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "INVITE");
		return;
	}

	

}