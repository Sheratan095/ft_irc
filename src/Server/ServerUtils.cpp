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
	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
			it->second->broadcastMessage(sender, "NICK", newNickname);
	}
}

// Send a message from client
void	Server::notifyQuit(Client *sender, const std::string &reason) const
{
	// default quitting reason
	std::string	message = (reason.empty()) ? "Client disconnected" : reason;

	for (std::map<std::string, Channel*>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (it->second->isClientInChannel(sender->getSocketFd()))
			it->second->broadcastMessage(sender, "QUIT", reason);
	}
}

SocketFd	Server::getSocketFd() const
{
	return (_socketFd);
}

void	Server::sendMessage(IRCMessage *message)
{
	ssize_t	bytesSent = send(message->_senderSocketFd, message->toString().c_str(), message->toString().size(), 0);

	if (bytesSent < 0)
		std::cerr << "Error sending message to socket fd: " << message->_senderSocketFd << std::endl;

	addIrcMessage(message);
}