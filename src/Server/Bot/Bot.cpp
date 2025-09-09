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
		return;

	std::string	prefix = getPrefix();

	std::vector<std::string>	lines;

	lines.push_back("Hello! I am the Magic 8-Ball 🎱");
	lines.push_back("Ask me any yes/no question by typing: !8ball <your question>");
	lines.push_back("Example: !8ball Will I pass exam rank 01?");
	lines.push_back("I will respond: yes sure as long as it doesn't come out get_next_line");

	for (size_t i = 0; i < lines.size(); ++i)
	{
		std::stringstream	ss;
	
		ss << ":" << prefix
		<< " PRIVMSG " << client->getNickname()
		<< " :" << lines[i] << "\r\n";
	
		sendMessage(client->getSocketFd(), ss.str());
	}
}