#include "Channel.hpp"


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