
#include "Server.hpp"

void	Server::userCmd(Client &client, const IRCMessage &message)
{
	if (message.parameters.size() != 4)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "USER");
		return;
	}

	if (client.checkRegistration())
	{
		sendResponse(client, ERR_ALREADYREGISTERED, "You are already registered");
		return;
	}


	client.setUsername(message.parameters[0]);
	// the second parameter is the hostname, which we ignore
	// the third parameter is the server name, which we ignore
	client.setNickname(message.parameters[3]);

	sendResponse(client, RPL_WELCOME, "Welcome to the IRC server!");
}