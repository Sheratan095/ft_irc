#include "Channel.hpp"

// :nick!user@host PRIVMSG #channel :Hello everyone!
// :nick!user@host	Prefix (who sent the message)
// PRIVMSG	Command
// #channel	Target (channel name)
// :Hello everyone!	Message text (trailing parameter)
void	Channel::broadcastMessage(Client *sender, const std::string &command, const std::string &message) const
{
	std::string	fullMessage = ":" + sender->getPrefix() + " " + command + " " + _name + " :" + message;

	IRCMessage	*msg = new IRCMessage(sender->getSocketFd(), sender->getPrefix(), command, { _name }, message);

	for (std::map<SocketFd, Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		_server->sendMessage(msg);

}
