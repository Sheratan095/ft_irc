#include "Server.hpp"

static bool	isChannelNameValid(const std::string &channelName);

void	Server::joinCmd(Client *client, const IRCMessage &message)
{
	if (message.parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "JOIN");
		return;
	}

	const std::string	&channelName = message.parameters[0];
	if (isChannelNameValid(channelName) == false)
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}


	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	Channel	*targetChannel;

	if (it == _channels.end())
	{
		// Create a new channel if it doesn't exist, the creator is admin by default
		Channel	*newChannel = new Channel(channelName, client);
		_channels[channelName] = newChannel;
		targetChannel = newChannel;
	
	}
	else
	{
		targetChannel = it->second;

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

	}

	notifyJoin(client, targetChannel);

	// The Topic isn't sent by default

	std::string userList = targetChannel->getNames();
	// = means that the channel is discoverable by everyone
	sendResponse(client, RPL_NAMREPLY, "= " + targetChannel->getName() + " :" + userList);

	// Fine della lista
	sendResponse(client, RPL_ENDOFNAMES, targetChannel->getName());

}

// must start with #, must contain something after #
static bool	isChannelNameValid(const std::string &channelName)
{
	// Check if the channel name starts with '#'
	if (channelName.empty() || channelName[0] != '#')
		return (false);

	// Check if the channel name contains at least one character after '#'
	if (channelName.size() == 1)
		return (false);

	if (channelName.size() > 50) // IRC standard limit for channel name length
		return (false);

	// Check for invalid characters in the rest of the name
	for (size_t i = 1; i < channelName.size(); ++i)
	{
		char	c = channelName[i];
		// Space, comma, and ASCII control characters are disallowed
		if (c == ' ' || c == ',' || static_cast<unsigned char>(c) < 0x20 || c == 0x7F)
			return (false);
	}

	return (true);
}