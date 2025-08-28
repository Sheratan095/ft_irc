#include "Server.hpp"

void	Server::whoCmd(Client *client, const IRCMessage &message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "WHO");
		return;
	}

	if (message.parameters.empty())
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "WHO");
		return;
	}

	// Channel
	if (startsWith(message.parameters[0], "#"))
	{
		const std::string	&channelName = toLower(message.parameters[0]);
		std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
		Channel	*targetChannel;

		if (it == _channels.end())
		{
			sendResponse(client, ERR_NOSUCHCHANNEL, channelName);
			return;
		}

		targetChannel = it->second;

		// Send the list of users in the channel
		// :irc.example.com 352 marco #chatroom alice host1 irc.example.com alice H :0 Alice Realname
		// :irc.example.com 352 marco #chatroom bob host2 irc.example.com bob H :0 Bob TheBuilder
		// :irc.example.com 315 marco #chatroom :End of WHO list

		// :server 352 <requester-nick> <channel> <username> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <realname>

		std::vector<std::string>	whoReply = targetChannel->getWho();

		for (std::vector<std::string>::iterator	it = whoReply.begin(); it != whoReply.end(); ++it)
			sendResponse(client, RPL_WHOREPLY, it->c_str());

		sendResponse(client, RPL_ENDOFWHO, targetChannel->getName());

	}
	else
	{
		// Look for client
		Client	*targetClient = findClientByName(message.parameters[0]);
		if (!targetClient)
		{
			sendResponse(client, ERR_NOSUCHNICK, message.parameters[0]);
			return;
		}

		// Send the client's information
		sendResponse(client, RPL_WHOREPLY, targetClient->getWho());

		sendResponse(client, RPL_ENDOFWHO, targetClient->getNickname());
	}
}
