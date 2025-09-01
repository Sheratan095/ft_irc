#include "Server.hpp"

// Send a message from client
void	Server::notifyNickChange(Client *sender, const std::string &oldNickname) const
{
	std::stringstream	ss;

	ss << ":" << oldNickname << " NICK :" << sender->getNickname() << "\r\n";

	std::string	message = ss.str();

	// Notify users in shared channels and with private conversations
	dispatchNotifications(sender, message);
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
	dispatchNotifications(sender, message);

}

void	Server::dispatchNotifications(Client *sender, const std::string &message) const
{
	// For every channel the sender is in, broadcast the message
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
		{
			it->second->broadcastMessage(message);
		}
	}

	 // Add clients in private conversations with the sender
	std::map<SocketFd, std::set<SocketFd> >::const_iterator	privIt = _privateConversations.find(sender->getSocketFd());
	if (privIt != _privateConversations.end())
	{
		for (std::set<SocketFd>::const_iterator pmIt = privIt->second.begin(); pmIt != privIt->second.end(); ++pmIt)
		{
			sendMessage(*pmIt, message);
		}
	}
}