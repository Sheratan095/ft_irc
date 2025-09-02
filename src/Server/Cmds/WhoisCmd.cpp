#include "Server.hpp"

void	Server::whoisCmd(Client *client, const IRCMessage *message)
{
	if (client->isRegistered() == false)
	{
		sendResponse(client, ERR_NOTREGISTERED, "WHOIS");
		return;
	}

	if (message->parameters.size() < 1)
	{
		sendResponse(client, ERR_NEEDMOREPARAMS, "WHOIS");
		return;
	}

	Client	*targetClient = findClientByName(message->parameters[0]);
	if (!targetClient)
	{
		sendResponse(client, ERR_NOSUCHNICK, message->parameters[0]);
		return;
	}

	// If the target client is found, send the WHOIS information
	sendResponse(client, RPL_WHOISUSER, formatWhoisUser(targetClient));
	sendResponse(client, RPL_WHOISCHANNELS, formatWhoisChannels(targetClient, _channels));
	sendResponse(client, RPL_WHOISSERVER, formatWhoisServer(targetClient));

	sendResponse(client, RPL_ENDOFWHOIS, targetClient->getNickname());
}

//311 <requester-nick> <target-nick> <username> <host> * :<realname>
std::string	Server::formatWhoisUser(Client *targetClient) const
{
	std::string	response = targetClient->getNickname() + " " + targetClient->getUsername() + " " + targetClient->getIpAddress() + " * :" + targetClient->getRealName();

	return (response);
}

//319 <requester-nick> <nick> :#chan1 @#chan2
std::string	Server::formatWhoisChannels(Client *targetClient, const std::map<std::string, Channel*> &channels) const
{
	std::string	channelList = targetClient->getNickname() + " :";

	for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel	*channel = it->second;
		if (channel->isClientInChannel(targetClient->getSocketFd()))
		{
			if (!channelList.empty())
				channelList += " ";
			channelList += channel->getName();
		}
	}

	return (channelList);
}

// 312 <requester-nick> <nick> <server> :<server info>
std::string	Server::formatWhoisServer(Client *targetClient) const
{
	std::string	response = targetClient->getNickname() + " " + SERVER_NAME + " :" + SERVER_INFO;

	return (response);
}