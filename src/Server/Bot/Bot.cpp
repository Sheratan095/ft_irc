#include "Bot.hpp"

Bot::Bot(const std::string &name) : _name(name)
{}

Bot::~Bot()
{}

std::string Bot::getName() const
{
	return (_name);
}

std::string	Bot::getPrefix() const
{
	return (_name + "!" + _name + "@" + _ipAddress);
}

void	Bot::setIpAddress(const std::string &ipAddress)
{
	_ipAddress = ipAddress;
}

bool	Bot::isRegistered() const
{
	return (!_ipAddress.empty() && !_name.empty());
}

void Bot::reciveMessage(Client *client, const std::string &message) const
{
	(void)client;
	(void)message;

	if (isRegistered() == false)
		return ;

	// Process the received message
}

void	Bot::sendWelcome(Client *client) const
{
	if (client == NULL)
		return ;

	std::stringstream	ss;

	ss << ":" << getPrefix()
	<< " PRIVMSG " << client->getNickname() // Target nick
	<< " :" << "ao zzaaa" << "\r\n";

	sendMessage(client->getSocketFd(), ss.str());
}
