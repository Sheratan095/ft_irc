#include "Server.hpp"

void	Server::privmsgCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "PRIVMSG");
		return;
	}

	if (message.parameters.size() < 1 || message.trailing.empty())
	{
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
		channel->notifyMsg(client, message.trailing);
	}
	else
	{
		// Handle private message
	}
}