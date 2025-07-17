#include "Server.hpp"

std::vector<IRCMessage>	Server::parseMessage(const std::string &message) const
{
	std::vector<IRCMessage>	messages;
	(void)message;

	return (messages);
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

		std::cout << "Command: " << msg.command;
		std::cout << "Prefix: " << msg.prefix << ", Command: " << msg.command;

		std::cout << ", Parameters: ";
		for (size_t i = 0; i < msg.parameters.size(); ++i)
			std::cout << msg.parameters[i] << " ";

		std::cout << std::endl;
	}

}
