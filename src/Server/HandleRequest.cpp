#include "Server.hpp"

void	Server::handleRequest(int client_fd)
{
	std::vector<IRCMessage>	messages;
	std::string				message = readMessageFromClient(client_fd);
	if (!message.empty())
	{
		messages = parseMessage(message);
		printRawMessage(messages);
	}

	messages = parseMessage(message);
	if (messages.empty())
	{
		std::cerr << "Error: No valid messages parsed from client fd: " << client_fd << std::endl;
		return;
	}

	for (size_t i = 0; i < messages.size(); ++i)
		switchCommand(messages[i], *_clients[client_fd]);

}

bool	Server::switchCommand(const IRCMessage &message, Client &client)
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
		removeClient(&client);
		return (true);
	}

	sendResponse(client, ERR_UNKNOWNCOMMAND, message.command);

	return (false);
}