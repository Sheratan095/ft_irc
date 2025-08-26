#include "Server.hpp"

Client*	Server::findClientByName(const std::string &nickname) const
{
	for (std::map<SocketFd, Client*>::const_iterator it = this->_clients.begin(); it!=this->_clients.end(); ++it)
	{
		if (insentiveStringCompare(it->second->getNickname(), nickname))
			return (it->second);
	}

	return (NULL);
}

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
	
	// Notify users in shared channels
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
		{
			// Get all members of the channel
			const std::map<SocketFd, Client*> &members = it->second->getMembers();
			
			// For each member, check if already notified
			for (std::map<SocketFd, Client*>::const_iterator memIt = members.begin(); 
				 memIt != members.end(); ++memIt)
			{
				if (notifiedClients.find(memIt->first) == notifiedClients.end())
				{
					// Not yet notified, send message
					sendMessage(memIt->first, message);
					notifiedClients[memIt->first] = true;
				}
			}
		}
	}
}

// TO DO
// Avoid duplicate notifications: Currently, if a user is in multiple channels, they might receive the same QUIT message multiple times. Consider tracking which clients have already been notified.
void	Server::notifyQuit(Client *sender, const std::string &reason) const
{
	// default quitting reason
	std::string	message = (reason.empty()) ? "Client disconnected" : reason;

	std::stringstream	ss;

	ss << ":" << sender->getNickname()
	<< "!" << sender->getUsername()
	<< "@" << sender->getIpAddress()
	<< " QUIT :" << message << "\r\n";

	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
			it->second->broadcastMessage(ss.str());
	}	
}

void	Server::notifyJoin(Client *client, Channel *channel) const
{
	// Notify all users in the channel
	std::stringstream	ss;
	ss << ":" << client->getNickname()
	<< "!" << client->getUsername()
	<< "@" << client->getIpAddress()
	<< " JOIN " << channel->getName()
	<< "\r\n";

	channel->broadcastMessage(ss.str());

	// Send back to the client the topic
	std::string	topic = channel->getTopic();
	if (topic.empty())
		sendResponse(client, RPL_NOTOPIC, channel->getName());
	else
		sendResponse(client, RPL_TOPIC, channel->getName() + " :" + topic);
}

void	Server::NotifyTopicChange(Client *client, Channel *channel) const
{
	std::string	topic = channel->getTopic();

	std::stringstream	ss;

	ss << ":" << client->getNickname()
	<< "!" << client->getUsername()
	<< "@" << client->getIpAddress()
	<< " TOPIC " << channel->getName()
	<< " :" << topic << "\r\n";

	channel->broadcastMessage(ss.str());
}