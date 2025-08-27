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

bool	Channel::isClientInChannel(SocketFd client_fd) const
{
	return (_members.find(client_fd) != _members.end());
}

bool	Channel::isInviteOnly() const
{
	return (_isInviteOnly);
}

bool	Channel::isChannelFull() const
{
	return (_members.size() >= _userLimit);
}

bool	Channel::isPasswordProtected() const
{
	return (_isPasswordProtected);
}

bool	Channel::isPasswordCorrect(const std::string &password) const
{
	return (_password == password);
}

bool	Channel::isClientInvited(SocketFd client_fd) const
{
	return (std::find(_invited.begin(), _invited.end(), client_fd) != _invited.end());
}

const std::string &Channel::getName() const
{
	return (_name);
}

const std::string	Channel::getUserList() const
{
	std::string	userList;

	for (std::map<SocketFd, Client*>::const_iterator pair = _members.begin(); pair != _members.end(); ++pair)
	{
		if (!userList.empty())
			userList += ", ";

		userList += pair->second->getNickname();
	}

	return (userList);
}

// Send the list of users in the channel
// :irc.example.com 352 marco #chatroom alice host1 irc.example.com alice H :0 Alice Realname
// :irc.example.com 352 marco #chatroom bob host2 irc.example.com bob H :0 Bob TheBuilder
// :irc.example.com 315 marco #chatroom :End of WHO list

// H here - G gone, * for irc server operator, @ for channel operator, + for voiced user
// :server 352 <requester-nick> <channel> <username> <host> <server> <nick> <H|G>[*][@][+] :<hopcount> <realname>
std::vector<std::string>	Channel::getWho() const
{
	std::vector<std::string>	whoList;

	for (std::map<SocketFd, Client*>::const_iterator pair = _members.begin(); pair != _members.end(); ++pair)
	{
		std::stringstream	ss;

		ss << this->getName()
			<< " " << pair->second->getUsername()
			<< " " << pair->second->getIpAddress() // Using IP address as
			<< " " << SERVER_NAME
			<< " " << pair->second->getNickname()
			<< " " << "H" //H = here, G = gone (away).
			<< (isClientOperator(pair->first)? "@": "") // @ if the user is a channel operator
			<< " :0 " // hop count
			<< pair->second->getRealName(); // realname is the nickname

		whoList.push_back(ss.str());

	}

	return (whoList);
}

std::string		Channel::getMode() const
{
	std::string	modes = "+";

	if (isInviteOnly())
		modes += "i";
	if (isPasswordProtected())
		modes += "k";
	if (isChannelFull())
		modes += "l";
	if (_istopicRestrictedToOps)
		modes += "t";
	if (_userLimit > 0)
		modes += "l";

	modes += "n"; // disallow messages from outside channel

	return (modes);
}

std::string	Channel::getNames() const
{
	std::string	namesList;

	for (std::map<SocketFd, Client*>::const_iterator pair = _members.begin(); pair != _members.end(); ++pair)
	{
		if (isClientOperator(pair->first))
			namesList += "@";

		namesList += pair->second->getNickname() + " ";
	}

	return (namesList);
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

