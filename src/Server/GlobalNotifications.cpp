#include "Server.hpp"

// Send a message from client
// isn't shown in private chats because it's considered a global event
void	Server::notifyNickChange(Client *sender, const std::string &oldNickname) const
{
	std::stringstream	ss;

	ss << ":" << oldNickname
	<< "!" << sender->getUsername()
	<< "@" << sender->getIpAddress()
	<< " NICK :" << sender->getNickname() << "\r\n";

	std::string	message = ss.str();

	// Notify users in shared channels and with private conversations
	dispatchNotifications(sender, message, true);
}

void	Server::notifyQuit(Client *sender, const std::string &reason) const
{
	std::stringstream	ss;

	ss << ":" << sender->getNickname()
	<< "!" << sender->getUsername()
	<< "@" << sender->getIpAddress()
	<< " QUIT :" << reason << "\r\n";

	std::string	message = ss.str();

	// Notify users in shared channels and with private conversations
	dispatchNotifications(sender, message, false);
}

// JUST ONE message for client is sent, than the client can display it in every tabs associated with that client
void	Server::dispatchNotifications(Client *sender, const std::string &message, bool includeSender) const
{
	std::set<SocketFd> result;

	// Always include the sender
	if (includeSender)
		result.insert(sender->getSocketFd());

	// For every channel the sender is in, collect all members
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin();
		it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
		{
			const std::map<SocketFd, Client*> &members = it->second->getMembers();
			for (std::map<SocketFd, Client*>::const_iterator memIt = members.begin();
				memIt != members.end(); ++memIt)
			{
				result.insert(memIt->first);
			}
		}
	}

	 // Add clients in private conversations with the sender
	std::map<SocketFd, std::set<SocketFd> >::const_iterator privIt = _privateConversations.find(sender->getSocketFd());
	if (privIt != _privateConversations.end())
	{
		for (std::set<SocketFd>::const_iterator pmIt = privIt->second.begin();
			pmIt != privIt->second.end(); ++pmIt)
		{
			result.insert(*pmIt);
		}
	}

	for (std::set<SocketFd>::const_iterator it = result.begin(); it != result.end(); ++it)
		sendMessage(*it, message);
}