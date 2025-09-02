#include "Server.hpp"

void	Server::modeCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "MODE");
		return;
	}

	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "MODE");
		return;
	}

	const std::string	&channelName = toLower(message.parameters[0]);

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

	// Only channel operators may set modes
	if (!targetChannel->isClientOperator(client->getSocketFd()))
	{
		sendResponse(client, ERR_CHANOPRIVSNEEDED, channelName);
		return;
	}
}
