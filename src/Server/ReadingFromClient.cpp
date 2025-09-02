#include "Server.hpp"

std::vector<IRCMessage> Server::parseMessage(const std::string &message) const
{
	std::vector<IRCMessage> messages;
	std::vector<std::string> lines;

	if (message.find("\r\n") != std::string::npos)
		lines = split(message, "\r\n");
	else
		lines = split(message, "\n");

	for (size_t i = 0; i < lines.size(); ++i)
	{
		const std::string &line = lines[i];
		if (line.empty())
			continue;

		IRCMessage msg;
		size_t pos = 0;
		std::string rest = line;

		// 1. Parse prefix
		if (rest[0] == ':')
		{
			pos = rest.find(' ');
			if (pos == std::string::npos)
				continue; // Invalid
			msg.prefix = rest.substr(1, pos - 1);
			rest = rest.substr(pos + 1);
		}

		// 2. Parse command
		pos = rest.find(' ');
		if (pos == std::string::npos)
		{
			msg.command = rest;
			messages.push_back(msg);
			continue;
		}
		msg.command = rest.substr(0, pos);
		rest = rest.substr(pos + 1);

		// 3. Parse parameters
		std::vector<std::string> params;
		while (!rest.empty())
		{
			if (rest[0] == ' ')
			{
				rest = rest.substr(1);
				continue;
			}
			if (rest[0] == ':')
			{
				// Trailing param — take the rest
				msg.trailing = rest.substr(1);
				break;
			}
			// Middle param
			pos = rest.find(' ');
			if (pos == std::string::npos)
			{
				params.push_back(rest);
				break;
			}
			params.push_back(rest.substr(0, pos));
			rest = rest.substr(pos + 1);
		}

		msg.parameters = params;
		messages.push_back(msg);
	}

	return messages;
}


std::string	Server::readMessageFromClient(int client_fd) const
{
	static char	buffer[1024];
	ssize_t	bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received < 0)
		std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
	else if (bytes_received == 0)
		std::cout << "Client " << client_fd << " is trying to disconnect in a strange way" << std::endl;

	buffer[bytes_received] = '\0'; // Null-terminate the received string

	return (std::string(buffer));
}

void	Server::printRawMessage(const std::vector<IRCMessage> &messages) const
{
	for (size_t i = 0; i < messages.size(); ++i)
	{
		const IRCMessage	&msg = messages[i];

		if (msg.prefix.empty())
			std::cout << "No prefix, " << std::endl;
		else
			std::cout << "Prefix: " << msg.prefix << ", " << std::endl;

		if (msg.command.empty())
			std::cout << "No command " << std::endl;
		else
			std::cout << "Command: " << msg.command << std::endl;

		if (msg.parameters.empty())
			std::cout << "No parameters " << std::endl;
		else
		{
			std::cout << "Parameters: ";
			for (size_t i = 0; i < msg.parameters.size(); ++i)
				std::cout << msg.parameters[i] << " " ;

			std::cout << std::endl;
		}

		if (!msg.trailing.empty())
			std::cout << "Trailing: " << msg.trailing << std::endl;

		std::cout << std::endl;

	}

}
