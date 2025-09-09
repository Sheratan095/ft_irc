#include "Bot.hpp"

bool BOT_RUNNING = true;

Bot::Bot(const std::string &server_ip, int server_port)
	: _server_ip(server_ip), _server_port(server_port)
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

bool	Bot::createSocket()
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
	{
		std::cerr << "Error creating socket" << std::endl;
		return false;
	}
	return true;
}

void	Bot::connectToServer(const std::string &password)
{
	struct sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_server_port);
	inet_pton(AF_INET, _server_ip.c_str(), &serverAddr.sin_addr);

	if (connect(_socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
	{
		std::cerr << "Error connecting to server at " << _server_ip << ":" << _server_port << std::endl;
		close(_socketFd);
		return;
	}

	std::cout << "Connected to IRC server at " << _server_ip << ":" << _server_port << std::endl;

	// IRC handshake
	if (!password.empty())
		sendMessage(_socketFd, "PASS " + password + "\r\n");
	sendMessage(_socketFd, "NICK " + _nick + "\r\n");
	sendMessage(_socketFd, "USER " + _nick + " 0 * :" + _nick + "\r\n");

	// main loop
	run();
}

void	Bot::run()
{
	while (BOT_RUNNING)
	{
		static char	buffer[1024];
		ssize_t		bytes_received = recv(_socketFd, buffer, sizeof(buffer) - 1, 0);

		// it goes on when the client try to connect with SSL
		if (bytes_received < 0)
		{
			std::cerr << "Error receiving data" << std::endl;
			break;
		}
		else if (bytes_received == 0)
		{
			std::cout << "Client " << _socketFd << " is trying to disconnect in a strange way" << std::endl;
			break;
		}

		buffer[bytes_received] = '\0'; // Null-terminate the received string

		std::vector<IRCMessage>	messages = parseMessage(std::string(buffer));

		for (size_t i = 0; i < messages.size(); ++i)
		{
			if (messages[i].command == "PRIVMSG")
				privmsgCmd(messages[i]);
			if (messages[i].command == "PING")
				sendMessage(_socketFd, "PONG :" + messages[i].trailing + "\r\n");
		}
	}
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

	sendMessage(_socketFd,"PRIVMSG " + getNickByPrefix(message.prefix) + " :" + reply + "\r\n");
}
