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
	// TO DO remove it
	(void)client;

	if (message.command == "CAP")
	{
		// TO DO
		// sendCustomErrorResponse(client.getSocketFd(), "CAP command not supported");

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

	// TO DO
	// sendErrorResponse(client.getSocketFd(), ERR_UNKNOWNCOMMAND, client.getNickname());

	return (false);
}