#include "Channel.hpp"

Channel::Channel(const std::string &name, Client *client): 
_name(name), _topic(""),
_isInviteOnly(false), _istopicRestrictedToOps(true),
_isPasswordProtected(false), _password(""), _userLimit(DEFAULT_USER_LIMIT_IN_CHANNEL)
{
	// Add the creator to the channel
	if (client)
	{
		_members[client->getSocketFd()] = client;
		_operators.push_back(client->getSocketFd());
	}
}

Channel::~Channel()
{}

bool	Channel::addClient(SocketFd client_fd, Client *client)
{
	if (_members.size() >= _userLimit)
		return (false); // User limit reached

	_members[client_fd] = client;
	return (true);
}

bool	Channel::removeClient(SocketFd client_fd)
{
	std::map<SocketFd, Client*>::iterator it = _members.find(client_fd);

	if (it != _members.end())
	{
		_members.erase(it);
		removeOperator(client_fd); // Also remove from operators if present
		return (true);
	}

	return (false); // Client not found
}

bool	Channel::inviteClient(SocketFd client_fd)
{
	std::list<SocketFd>::iterator it = std::find(_invited.begin(), _invited.end(), client_fd);

	if (it == _invited.end())
	{
		_invited.push_back(client_fd);
		return (true); // Client invited successfully
	}

	return (false); // Client already invited
}

bool	Channel::addOperator(SocketFd client_fd)
{
	std::list<SocketFd>::iterator it = std::find(_operators.begin(), _operators.end(), client_fd);

	if (it == _operators.end())
	{
		_operators.push_back(client_fd);
		return (true); // Operator added successfully
	}

	return (false); // Client is already an operator
}

bool	Channel::removeOperator(SocketFd client_fd)
{
	std::list<SocketFd>::iterator it = std::find(_operators.begin(), _operators.end(), client_fd);

	if (it != _operators.end())
	{
		_operators.erase(it);
		return (true); // Operator removed successfully
	}

	return (false); // Client is not an operator
}

void	Channel::notifyJoin(Client *client) const
{
	// Notify all users in the channel
	std::stringstream	ss;
	ss << ":" << client->getNickname()
	<< "!" << client->getUsername()
	<< "@" << client->getIpAddress()
	<< " JOIN " << this->getName()
	<< "\r\n";

	this->broadcastMessage(ss.str());
}

void	Channel::notifyTopicChange(Client *client) const
{
	std::string	topic = this->getTopic();

	std::stringstream	ss;

	ss << ":" << client->getNickname()
	<< "!" << client->getUsername()
	<< "@" << client->getIpAddress()
	<< " TOPIC " << this->getName()
	<< " :" << topic << "\r\n";

	this->broadcastMessage(ss.str());
}

//:<kicker>!<kicker_user>@<kicker_host> KICK <channel> <target> :<reason>
void	Channel::notifyKick(Client *kickerClient, Client *kickedClient, const std::string &reason) const
{
	std::stringstream	ss;

	ss << ":" << kickerClient->getNickname()
	<< "!" << kickerClient->getUsername()
	<< "@" << kickerClient->getIpAddress()
	<< " KICK " << this->getName()
	<< " " << kickedClient->getNickname()
	<< " :" << reason << "\r\n";

	this->broadcastMessage(ss.str());
}


// :nick!user@host PRIVMSG #channel :Hello everyone!
// :nick!user@host	Prefix (who sent the message)
// PRIVMSG	Command
// #channel	Target (channel name)
// :Hello everyone!	Message text (trailing parameter)
void	Channel::broadcastMessage(const std::string &message) const
{
	for (std::map<SocketFd, Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		sendMessage(it->first, message);
}
