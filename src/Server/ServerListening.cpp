#include "Server.hpp"

bool	Server::startListening()
{
	if (listen(_socket_fd, SOMAXCONN) == -1)
	{
		// FAILED TO LISTEN ON SOCKET
		close(_socket_fd);
		return (false);
	}
	return (true);
}

void	Server::Listen()
{
	std::cout << "Server is listening on " << _ip << ":" << _port << std::endl;

	// Configure socket timeout to prevent accept() from blocking indefinitely
	// This allows the server to periodically check SERVER_RUNNING flag
	struct timeval	timeout;
	timeout.tv_sec = 1;		// 1 second timeout
	timeout.tv_usec = 0;	// 0 microseconds
	setsockopt(_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	while (SERVER_RUNNING)
	{
		// Prepare client address structure to store incoming connection info
		struct sockaddr_in	client_addr;
		socklen_t			client_len = sizeof(client_addr);

		// Accept incoming client connection (blocks until connection or timeout)
		int	client_fd = accept(_socket_fd, (struct sockaddr*)&client_addr, &client_len);
		
		// Check if accept() failed, it could be due to timeout to check SERVER_RUNNING
		if (client_fd == -1)
		{
			// check if the server has been stopped
			if (!SERVER_RUNNING)
			{
				std::cout << "Server shutting down..." << std::endl;
				break;
			}
	
			// If it's just a timeout or other error, continue listening
			continue;
		}

		// Handle the connected client
		handleClient(client_fd);
	}
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