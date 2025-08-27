#include "Channel.hpp"

const std::map<SocketFd, Client*>& Channel::getMembers() const
{
	return (_members);
}

const std::string	&Channel::getTopic() const
{
	return (_topic);
}

void	Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void	Channel::setInviteOnly(bool isInviteOnly)
{
	_isInviteOnly = isInviteOnly;
}

void	Channel::setTopicRestrictedToOps(bool istopicRestrictedToOps)
{
	_istopicRestrictedToOps = istopicRestrictedToOps;
}

void	Channel::setPasswordProtected(bool isPasswordProtected, const std::string &password)
{
	_isPasswordProtected = isPasswordProtected;

	if (isPasswordProtected)
		_password = password;
	else
		_password.clear();
}

void	Channel::setUserLimit(size_t userLimit)
{
	if (userLimit > 0)
		_userLimit = userLimit;
	else
		_userLimit = DEFAULT_USER_LIMIT_IN_CHANNEL;
}

bool	Channel::isClientOperator(SocketFd client_fd) const
{
	return (std::find(_operators.begin(), _operators.end(), client_fd) != _operators.end());
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