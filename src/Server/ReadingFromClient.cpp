#include "Server.hpp"

char	*Server::readMessageFromClient(int client_fd)
{
	static char	buffer[1024];
	ssize_t	bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received < 0)
	{
		std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
		return (NULL); // Error occurred
	}
	else if (bytes_received == 0)
	{
		std::cout << "Client disconnected gracefully" << std::endl;
		return (NULL); // Client closed connection
	}

	buffer[bytes_received] = '\0'; // Null-terminate the received string

	return (buffer); // Return the received message
}

void	Server::printRawMessage(int bytes_received, char *buffer)
{
	// Debug: Print raw bytes received and message length
	std::cout << "Bytes received: " << bytes_received << std::endl;
	std::cout << "Raw message (with escape chars): ";

	for (int i = 0; i < bytes_received; i++)
	{
		if (buffer[i] == '\n')
			std::cout << "\\n";
		else if (buffer[i] == '\r')
			std::cout << "\\r";
		else
			std::cout << buffer[i];
	}

	std::cout << std::endl;
}
