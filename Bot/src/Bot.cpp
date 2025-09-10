#include "Bot.hpp"

bool BOT_RUNNING = true;

Bot::Bot(const std::string &server_ip, int server_port) : _server_ip(server_ip), _server_port(server_port)
{
	_nick = "MagicBall";

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
{
	_responses.clear();
}

void	Bot::run()
{
	struct pollfd	pfd;
	pfd.fd = _socketFd;
	pfd.events = POLLIN;

	while (true)
	{
		int	ret = poll(&pfd, 1, 100); // 100ms timeout
		if (ret < 0)
		{
			if (BOT_RUNNING == true)
				std::cerr << "Poll error" << std::endl;
			break;
		}
		else if (ret == 0)
		{
			// Timeout, no data yet, just continue
			continue;
		}

		if (pfd.revents & POLLIN)
		{
			char	buffer[1024];
			ssize_t	bytes_received = recv(_socketFd, buffer, sizeof(buffer) - 1, 0);
			if (bytes_received <= 0)
			{
				std::cout << "Server closed connection or error" << std::endl;
				break;
			}

			buffer[bytes_received] = '\0';

			std::vector<IRCMessage>	messages = parseMessage(std::string(buffer));
			printRawMessage(messages);
			for (size_t i = 0; i < messages.size(); ++i)
				switchCommand(messages[i]);
		}
	}

	close(_socketFd);
}

void	Bot::switchCommand(const IRCMessage &command)
{
	if (command.command == "PRIVMSG")
		privmsgCmd(command);
	if (command.command == "PING")
		sendMessage(_socketFd, "PONG :" + command.trailing + "\r\n");
	if (command.command == "INVITE")
		sendMessage(_socketFd, "JOIN " + command.trailing + "\r\n");
}

void Bot::privmsgCmd(const IRCMessage &message) const
{
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

	//If the message was sent in a channel, reply to the channel, else reply to the user
	if (message.parameters.size() > 0 && startsWith(message.parameters[0], "#"))
		sendMessage(_socketFd,"PRIVMSG " + message.parameters[0] + " :" + reply + "\r\n");
	else
		sendMessage(_socketFd,"PRIVMSG " + getNickByPrefix(message.prefix) + " :" + reply + "\r\n");
}

std::string	Bot::getBotUsage() const
{
	return ("!8ball <your yes/no question> with a random answer.");

}