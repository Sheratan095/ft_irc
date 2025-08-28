#include "Server.hpp"

void	Server::inviteCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "INVITE");
		return;
	}

	if (message.parameters.size() < 2)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "INVITE");
		return;
	}

	// The client to invite must be connected to the server
	Client	*clientInvited = findClientByName(message.parameters[0]);
	if (clientInvited == NULL)
	{
		sendResponse(client, ERR_NOSUCHNICK, message.parameters[0]);
		return;
	}

	// The channel to invite the client to must exist
	std::map<std::string, Channel*>::iterator	it = _channels.find(message.parameters[1]);
	if (it == _channels.end())
	{
		sendResponse(client, ERR_NOSUCHCHANNEL, message.parameters[1]);
		return;
	}
	Channel *targetChannel = it->second;

	// The inviter must be in the channel
	if (targetChannel->isClientInChannel(client->getSocketFd()) == false)
	{
		sendResponse(client, ERR_NOTONCHANNEL, targetChannel->getName());
		return;
	}

	// If the channel is invite only, the inviter must be a channel operator
	if (targetChannel->isInviteOnly() && !targetChannel->isClientOperator(client->getSocketFd()))
	{
		sendResponse(client, ERR_INVITEONLYCHAN, targetChannel->getName());
		return;
	}

	// The invited client must not already be in the channel
	if (targetChannel->isClientInChannel(clientInvited->getSocketFd()))
	{
		sendResponse(client, ERR_USERONCHANNEL, clientInvited->getNickname());
		return;
	}

	targetChannel->inviteClient(clientInvited->getSocketFd());

	sendResponse(client, RPL_INVITE, clientInvited->getNickname() + targetChannel->getName());

}