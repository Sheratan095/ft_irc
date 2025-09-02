#include "Server.hpp"

// Utility: replace all substrings
static void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

std::vector<IRCMessage> Server::parseMessage(const std::string &message) const
{
	std::vector<IRCMessage> messages;

	// 1. Normalize line endings (\r\n → \n, \r → \n)
	std::string normalized = message;
	replaceAll(normalized, "\r\n", "\n");
	replaceAll(normalized, "\r", "\n");

	// 2. Split into lines
	std::vector<std::string> lines = split(normalized, "\n");

	for (size_t i = 0; i < lines.size(); ++i)
	{
		const std::string &line = lines[i];
		if (line.empty())
			continue;

		IRCMessage msg;
		size_t pos = 0;
		std::string rest = line;

		// 3. Prefix (optional)
		if (rest[0] == ':')
		{
			pos = rest.find(' ');
			if (pos == std::string::npos)
				continue; // Invalid line
			msg.prefix = rest.substr(1, pos - 1);
			rest = rest.substr(pos + 1);
		}

		// 4. Command
		pos = rest.find(' ');
		if (pos == std::string::npos)
		{
			msg.command = rest;
			messages.push_back(msg);
			continue;
		}
		msg.command = rest.substr(0, pos);
		rest = rest.substr(pos + 1);

		// 5. Parameters
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
				// Trailing param — keep raw (important for CTCP/DCC)
				msg.trailing = rest.substr(1);
				break;
			}
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

	return (std::string(buffer, bytes_received));
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
