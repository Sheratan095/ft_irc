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
void	Server::notifyNickChange(Client *sender, const std::string &newNickname) const
{
	std::stringstream ss;
	ss << ":" << sender->getNickname() << " NICK :" << newNickname;

	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
			it->second->broadcastMessage(ss.str());
	}
}

// Send a message from client
void	Server::notifyQuit(Client *sender, const std::string &reason) const
{
	// default quitting reason
	std::string	message = (reason.empty()) ? "Client disconnected" : reason;

	std::stringstream ss;
	ss << ":" << sender->getNickname() << " QUIT :" << message;

	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
			it->second->broadcastMessage(ss.str());
	}
}

void	Server::notifyJoin(Client *client, const std::string &channelName) const
{
	std::stringstream ss;
	ss << ":" << client->getNickname() << " JOIN :" << channelName;

	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(client->getSocketFd()))
			it->second->broadcastMessage(ss.str());
	}
}