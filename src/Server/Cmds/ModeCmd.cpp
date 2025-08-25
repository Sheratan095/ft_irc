#include "Server.hpp"

void	Server::modeCmd(Client *client, const IRCMessage &message)
{
	(void)client; // Currently not used, but can be implemented later
	(void)message; // Currently not used, but can be implemented later

	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "MODE");
		return;
	}

	const std::string	&channelName = message.parameters[0];

	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	Channel	*targetChannel;

	if (it == _channels.end())
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}

	targetChannel = it->second;

	// check that the client is connected to the channel
	if (targetChannel->isClientInChannel(client->getSocketFd()) == false)
	{
		sendResponse(client, ERR_NOTONCHANNEL, channelName);
		return;
	}

	if (message.parameters.size() == 1)
	{
		sendResponse(client, RPL_CHANNELMODEIS, channelName + " " + targetChannel->getMode());
		return;
	}
}
