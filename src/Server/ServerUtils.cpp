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

