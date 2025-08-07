#include "Channel.hpp"

Channel::Channel(const std::string &name): 
_name(name), _topic(""),
_isInviteOnly(false), _istopicRestrictedToOps(true),
_isPasswordProtected(false), _password(""), _userLimit(DEFAULT_USER_LIMIT_IN_CHANNEL)
{}

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

void	Channel::broadcastMessage(Client *sender, const std::string &message)
{
	sendMessage(sender->getSocketFd(), message);
}
