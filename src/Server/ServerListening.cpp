#include "Server.hpp"

bool	Server::startListening()
{
	if (listen(socket_fd, SOMAXCONN) == -1)
	{
		// FAILED TO LISTEN ON SOCKET
		close(socket_fd);
		return (false);
	}
	return (true);
}

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

			// Send a proper IRC response instead of echoing
			std::string response = ":server 0001 :Welcome to the IRC Network\r\n";
			ssize_t sent = send(client_fd, response.c_str(), response.length(), 0);
			if (sent == -1)
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