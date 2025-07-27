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
		switchCommand(messages[i], client_fd);


}

bool	Server::switchCommand(const IRCMessage &message, int client_fd)
{
	if (message.command == "CAP")
	{
		sendErrorResponse(client_fd, "CAP command not supported");
		// Handle JOIN command
		return (true);
	}

	if (message.command == "PASS")
	{
		// Handle PASS command
		return (true);
	}

	if (message.command == "NICK")
	{
		// Handle NICK command
		return (true);
	}

	if (message.command == "USER")
	{
		// Handle JOIN command
		return (true);
	}

	sendErrorResponse(client_fd, ERR_UNKNOWNCOMMAND);

	return (false);
}