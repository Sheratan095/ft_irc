#include "Server.hpp"

void	Server::privmsgCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "PRIVMSG");
		return;
	}

	std::string messageText = message.trailing;
	if (messageText.empty() && message.parameters.size() > 1)
	{
		// Rebuild message from parameters[1..]
		messageText = "";
		for (size_t i = 1; i < message.parameters.size(); ++i) {
			if (i > 1) messageText += " ";
			messageText += message.parameters[i];
		}
	}

	if (messageText.empty()) {
		sendResponse(client, ERR_NEEDMOREPARAMS, "PRIVMSG");
		return;
	}

	std::string	target = message.parameters[0];
	if (startsWith(target, "#"))
	{
		// Handle channel message
		std::map<std::string, Channel*>::iterator	it = _channels.find(toLower(target));
		if (it == _channels.end())
		{
			sendResponse(client, ERR_NOSUCHCHANNEL, target);
			return;
		}

		Channel	*channel = it->second;
		if (channel->isClientInChannel(client->getSocketFd()) == false)
		{
			sendResponse(client, ERR_NOTONCHANNEL, target);
			return;
		}

		channel->notifyMsg(client, message.trailing);
	}
	else
	{
		// Handle private message

		Client	*recipient = findClientByName(target);
		if (!recipient)
		{
			sendResponse(client, ERR_NOSUCHNICK, target);
			return;
		}

		recipient->sendPrivMessage(client, message.trailing);

		//Update the map with private message
		//std::map<SocketFd, std::set<SocketFd> > _privateConversations;

		_privateConversations[client->getSocketFd()].insert(recipient->getSocketFd());
		_privateConversations[recipient->getSocketFd()].insert(client->getSocketFd());
	}
}