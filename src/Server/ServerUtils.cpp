#include "Server.hpp"

Client*	Server::findClientByName(const std::string &nickname) const
{
	for (std::map<SocketFd, Client*>::const_iterator it = this->_clients.begin(); it!=this->_clients.end(); ++it)
	{
		if (insensitiveStringCompare(it->second->getNickname(), nickname))
			return (it->second);
	}

	return (NULL);
}

void Server::sendInviteMessage(Client *inviter, Client *invited, Channel *channel) const
{
	std::ostringstream	oss;

	// Format: :<inviter>!<user>@<host> INVITE <invited-nick> :<channel>
	oss << ":" << inviter->getNickname()
		<< "!" << inviter->getUsername()
		<< "@" << inviter->getIpAddress()
		<< " INVITE " << invited->getNickname()
		<< " :" << channel->getName()
		<< "\r\n";

	sendMessage(invited->getSocketFd(), oss.str());
}