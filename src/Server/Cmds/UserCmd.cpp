
#include "Server.hpp"

void	Server::userCmd(Client *client, const IRCMessage &message)
{
	// if the client is already registered before the message: error
	if (client->isRegistered())
	{
		sendResponse(client, ERR_ALREADYREGISTERED, "");
		return;
	}

	if (message.parameters.size() < 2 || message.trailing.empty())
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "USER");
		return;
	}

	client->setUsername(message.parameters[0]);
	// the second parameter is the hostname, which we ignore
	// the third parameter is the server name, which we ignore
	client->setRealName(message.trailing);

	// if the client is now FULL registered, send a welcome message
	if (client->isRegistered())
		sendResponse(client, RPL_WELCOME, "");
}