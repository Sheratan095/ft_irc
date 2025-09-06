#include "Server.hpp"

void	Server::handleRequest(int client_fd)
{
	std::vector<IRCMessage>	messages;
	std::string				message = readMessageFromClient(client_fd);
	
	if (message.empty())
	{
		quitCmd(_clients[client_fd], "Client disconnected");

		return ;
	}
	
	messages = parseMessage(message);
	printRawMessage(messages);

	if (messages.empty())
	{
		std::cerr << "Error: empty message sent from fd: " << client_fd << std::endl << std::endl;
		return;
	}

	for (size_t i = 0; i < messages.size(); ++i)
		switchCommand(messages[i], _clients[client_fd]);

}

bool	Server::switchCommand(const IRCMessage &message, Client *client)
{

	if (message.command == "CAP")
	{
		sendResponse(client, ERR_UNKNOWNCOMMAND, message.command);
		return (true);
	}

	if (message.command == "PASS")
	{
		passCmd(client, message);
		return (true);
	}

	if (message.command == "NICK")
	{
		nickCmd(client, message);
		return (true);
	}

	if (message.command == "USER")
	{
		userCmd(client, message);
		return (true);
	}

	if (message.command == "QUIT")
	{
		quitCmd(client, message);
		return (true);
	}

	if (message.command == "JOIN")
	{
		joinCmd(client, message);
		return (true);
	}

	if (message.command == "MODE")
	{
		modeCmd(client, message);
		return (true);
	}

	if (message.command == "WHO")
	{
		whoCmd(client, message);
		return (true);
	}

	if (message.command == "TOPIC")
	{
		topicCmd(client, message);
		return (true);
	}

	if (message.command == "INVITE")
	{
		inviteCmd(client, message);
		return (true);
	}

	if (message.command == "KICK")
	{
		kickCmd(client, message);
		return (true);
	}

	if (message.command == "PRIVMSG")
	{
		privmsgCmd(client, message);
		return (true);
	}

	if (message.command == "WHOIS")
	{
		whoisCmd(client, &message);
		return (true);
	}

	sendResponse(client, ERR_UNKNOWNCOMMAND, message.command);

	return (false);
}