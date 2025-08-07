#include "Channel.hpp"

// :nick!user@host PRIVMSG #channel :Hello everyone!
// :nick!user@host	Prefix (who sent the message)
// PRIVMSG	Command
// #channel	Target (channel name)
// :Hello everyone!	Message text (trailing parameter)
void	Channel::broadcastMessage(Client *sender, const std::string &command, Channel *channel, const std::string &message) const
{

}
