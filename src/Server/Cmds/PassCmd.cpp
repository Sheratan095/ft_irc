#include "Server.hpp"

void	Server::passCmd(Client *client, const IRCMessage &message)
{
	// if the client is already registered before the message: error
	if (client->isRegistered())
	{
		sendResponse(client, ERR_ALREADYREGISTERED, "");
		return;
	}

	if (message.parameters.size() != 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "PASS");
		return;
	}

	if (message.parameters[0] != _password)
	{
		sendResponse(client, ERR_PASSWDMISMATCH, "");
		return;
	}

	client->setAuthenticated();

	// if the client is now FULL registered, send a welcome message
	if (client->isRegistered())
		sendWelcomeMessages(client);
}