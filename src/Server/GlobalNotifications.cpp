#include "Server.hpp"

// Send a message from client
void	Server::notifyNickChange(Client *sender, const std::string &oldNickname) const
{
	std::stringstream	ss;

	ss << ":" << oldNickname << " NICK :" << sender->getNickname() << "\r\n";

	std::string message = ss.str();
	
	// Set to track clients who've already been notified
	std::map<SocketFd, bool> notifiedClients;
	
	// Always notify the user themselves
	sendMessage(sender->getSocketFd(), message);
	notifiedClients[sender->getSocketFd()] = true;

	// Notify users in shared channels and with private conversations
	std::set<SocketFd> clientsToNotify = getClientsToNotify(sender);
	
	for (std::set<SocketFd>::const_iterator it = clientsToNotify.begin(); it != clientsToNotify.end(); ++it)
		sendMessage(*it, message);
}

void	Server::notifyQuit(Client *sender, const std::string &reason) const
{
	std::stringstream	ss;

	ss << ":" << sender->getNickname()
	<< "!" << sender->getUsername()
	<< "@" << sender->getIpAddress()
	<< " QUIT :" << reason << "\r\n";

	// Notify users in shared channels and with private conversations
	std::set<SocketFd> clientsToNotify = getClientsToNotify(sender);
	
	std::string formattedMessage = ss.str();
	for (std::set<SocketFd>::const_iterator it = clientsToNotify.begin(); it != clientsToNotify.end(); ++it)
		sendMessage(*it, formattedMessage);
}

// retrieve all the clients that shar
// Collect all clients that must be notified of a nick change
std::set<SocketFd> Server::getClientsToNotify(Client *sender) const
{
	std::set<SocketFd> result;

	// Always include the sender
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

	return (result);
}