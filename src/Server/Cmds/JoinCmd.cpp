#include "Server.hpp"

void	Server::joinCmd(Client *client, const IRCMessage &message)
{
	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "JOIN");
		return;
	}

	const std::string	&channelName = message.parameters[0];
	if (channelName.empty() || channelName[0] != '#')
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}


	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);

	if (it == _channels.end())
	{
		// Create a new channel if it doesn't exist, the creator is admin by default
		Channel	*newChannel = new Channel(channelName, client);
		_channels[channelName] = newChannel;

		return ;
	}

	Channel	*targetChannel = it->second;

	// try to join the channel
	if (targetChannel->isInviteOnly())
	{
		if (!targetChannel->isClientInvited(client->getSocketFd()))
		{
			sendResponse(client, ERR_INVITEONLYCHAN, channelName);
			return;
		}
	}

	if (targetChannel->isChannelFull())
	{
		sendResponse(client, ERR_CHANNELISFULL, channelName);
		return;
	}

	if (targetChannel->isPasswordProtected())
	{
		if (message.parameters.size() < 2 || !targetChannel->isPasswordCorrect(message.parameters[1]))
		{
			sendResponse(client, ERR_BADCHANNELKEY, channelName);
			return;
		}
	}

	targetChannel->addClient(client->getSocketFd(), client);

	notifyJoin(client, channelName);

	// TO DO notify other clients in the channel
	// send topic to the just connected client?
	// send name list to the just connected client?
}