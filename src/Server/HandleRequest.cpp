#include "Server.hpp"

void	Server::handleClient(int client_fd)
{
	std::cout << std::endl << "Client connected" << std::endl;

	// Keep connection open and handle multiple messages
	while (true)
	{
		char	buffer[1024];
		ssize_t	bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

		if (bytes_received > 0)
		{
			buffer[bytes_received] = '\0';

			printRawMessage(bytes_received, buffer);

			// Send a proper IRC response - Method 1: Using + operator
			std::string response = getResponseByCode(RPL_WELCOME);

			ssize_t bytes_sent = send(client_fd, response.c_str(), response.length(), 0);
			if (bytes_sent == -1)
			{
				std::cout << "Failed to send response to client" << std::endl;
				break;
			}
		}
		else if (bytes_received == 0)
		{
			std::cout << "Client disconnected gracefully" << std::endl;
			break;  // Client closed connection
		}
		else
		{
			// Check if it's just a timeout (errno 11 = EAGAIN/EWOULDBLOCK)
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				// Timeout occurred, continue waiting for data
				continue;
			}
			std::cout << "Error receiving data, errno: " << errno << " (" << strerror(errno) << ")" << std::endl;
			break;  // Actual error occurred, close connection
		}
	}
	
	close(client_fd);
	std::cout << "Client connection closed" << std::endl;
}

// void	Server::switchCommand()
// {
// 	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (socket_fd == -1)
// 	{
// 		// SOCKET CREATION FAILED
// 		throw SocketException();
// 	}
// }