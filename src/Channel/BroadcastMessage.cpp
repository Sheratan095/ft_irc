#include "Channel.hpp"

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
