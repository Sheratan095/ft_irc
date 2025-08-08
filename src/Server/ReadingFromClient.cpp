#include "Server.hpp"

std::vector<IRCMessage> Server::parseMessage(const std::string &message)
{
	std::vector<IRCMessage> messages;
	std::vector<std::string> lines = split(message, "\r\n");

	IRCMessage	*newIrcMessage;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].empty())
			continue;

		newIrcMessage = new IRCMessage(_pollFds[0].fd, lines[i]); // Assuming _pollFds[0].fd is the sender's socket fd

		messages.push_back(*newIrcMessage);

		addIrcMessage(newIrcMessage);
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
		std::cout << "Client disconnected gracefully" << std::endl;

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
