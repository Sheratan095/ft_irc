#include "Channel.hpp"

//----------------------------i----------------------------
void	Channel::setInviteOnly()
{
	_isInviteOnly = true;
}

void	Channel::removeInviteOnly()
{
	_isInviteOnly = false;
}

bool	Channel::isInviteOnly() const
{
	return (_isInviteOnly);
}

//----------------------------t----------------------------
void	Channel::setTopicRestriction()
{
	_istopicRestrictedToOps = true;
}

void	Channel::removeTopicRestriction()
{
	_istopicRestrictedToOps = false;
}

bool	Channel::isTopicRestrictedToOps() const
{
	return (_istopicRestrictedToOps);
}

//----------------------------k----------------------------
void	Channel::setPassword(const std::string &password)
{
	_isPasswordProtected = true;
	_password = password;
}

void	Channel::removePassword()
{
	_isPasswordProtected = false;
	_password.clear();
}

bool	Channel::isPasswordProtected() const
{
	return (_isPasswordProtected);
}

bool	Channel::isPasswordCorrect(const std::string &password) const
{
	return (_password == password);
}

//----------------------------l----------------------------
void	Channel::setUserLimit(size_t userLimit)
{
	if (_userLimit < DEFAULT_USER_LIMIT_IN_CHANNEL)
		_userLimit = userLimit;
	else
		_userLimit = DEFAULT_USER_LIMIT_IN_CHANNEL;

	_isUserLimited = true;
}

void	Channel::removeUserLimit()
{
	_isUserLimited = false;
	_userLimit = DEFAULT_USER_LIMIT_IN_CHANNEL;
}

bool	Channel::isUserLimited() const
{
	return (_isUserLimited);
}

std::string		Channel::getMode() const
{
	std::string	modes = "+";

	if (_isInviteOnly)
		modes += "i";
	if (_istopicRestrictedToOps)
		modes += "t";
	if (_isPasswordProtected)
		modes += "k";
	if (_isUserLimited)
		modes += "l";

	modes += "n"; // disallow messages from outside channel

	return (modes);
}
