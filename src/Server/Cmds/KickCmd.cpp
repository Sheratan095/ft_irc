#include "Server.hpp"

void	Server::kickCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "");
		return ;
	}

	if (message.parameters.size() < 2)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "KICK");
		return;
	}

	const std::string&	channelName =  toLower(message.parameters[0]);
	const std::string&	targetNickname = message.parameters[1];
	const std::string&	reason = message.parameters.size() == 3 ? message.parameters[2] : "";

	// Check if the channel exist
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	if (it == _channels.end())
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
		return;
	}
	Channel *targetChannel = it->second;

	// The client to kick must be connected to the server
	Client	*clientToKick = findClientByName(targetNickname);
	if (clientToKick == NULL)
	{
		sendResponse(client, ERR_NOSUCHNICK, targetNickname);
		return;
	}

	// The kicker must be in the channel
	if (targetChannel->isClientInChannel(client->getSocketFd()) == false)
	{
		sendResponse(client, ERR_NOTONCHANNEL, targetChannel->getName());
		return;
	}

	// The kicked must be in the channel
	if (targetChannel->isClientInChannel(clientToKick->getSocketFd()) == false)
	{
		sendResponse(client, ERR_USERNOTINCHANNEL, clientToKick->getNickname());
		return;
	}

	// Only operators can KICK users from a channel
	if (targetChannel->isClientOperator(client->getSocketFd()) == false)
	{
		sendResponse(client, ERR_CHANOPRIVSNEEDED, targetChannel->getName());
		return;
	}

	targetChannel->notifyKick(client, clientToKick, reason);

	targetChannel->removeClient(clientToKick->getSocketFd());
}