#include "Server.hpp"

// FORMAT: /LIST
void	Server::listCmd(Client *client, const IRCMessage &message)
{
	(void)message; // Unused parameter

	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "INVITE");
		return;
	}

	sendResponse(client, RPL_LISTSTART, "");
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel	*channel = it->second;
		sendResponse(client, RPL_LIST, channel->getName() + " " + intToString(channel->getClientCount()) + " :" + channel->getTopic());
	}

	sendResponse(client, RPL_LISTEND, "");
}