#include "Bot.hpp"

Bot::Bot(const std::string &name) : _name(name)
{
	_responses.push_back("It is certain.");
	_responses.push_back("It is decidedly so.");
	_responses.push_back("Without a doubt.");
	_responses.push_back("Yes - definitely.");
	_responses.push_back("You may rely on it.");
	_responses.push_back("As I see it, yes.");
	_responses.push_back("Most likely.");
	_responses.push_back("Outlook good.");
	_responses.push_back("Yes.");
	_responses.push_back("Signs point to yes.");

	_responses.push_back("Reply hazy, try again.");
	_responses.push_back("Ask again later.");
	_responses.push_back("Better not tell you now.");
	_responses.push_back("Cannot predict now.");
	_responses.push_back("Concentrate and ask again.");

	_responses.push_back("Don't count on it.");
	_responses.push_back("My reply is no.");
	_responses.push_back("My sources say no.");
	_responses.push_back("Outlook not so good.");
	_responses.push_back("Very doubtful.");
}

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

void Bot::receiveMessage(Client *client, const IRCMessage &message) const
{
	if (isRegistered() == false)
		return ;

	if (message.trailing.empty())
		return ;

	std::string	reply;

	if (startsWith(message.trailing, "!8ball") == false)
	{
		reply = "Usage: !8ball <your yes/no question>";
		
	}
	else if (message.trailing == "!8ball" || message.trailing == "!8ball ")
	{
		reply = "Please ask a yes/no question after the command.";
	}
	else if (message.trailing.find("?") == std::string::npos)
	{
		reply = "That doesn't look like a question. Please ask a yes/no question. (include '?')";
	}
	else
	{
		static int	oldResponseIndex = -1;
		int			responseIndex = rand() % _responses.size();
		while ((responseIndex = rand() % _responses.size()) == oldResponseIndex)
			;

		oldResponseIndex = responseIndex;

		reply = _responses[responseIndex];
	}

	std::stringstream	ss;

	ss << ":" << getPrefix()
	<< " PRIVMSG " << client->getNickname()
	<< " :" << reply << "\r\n";

	sendMessage(client->getSocketFd(), ss.str());
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
