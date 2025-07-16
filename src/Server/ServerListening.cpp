#include "Server.hpp"

bool	Server::startListening()
{
	if (listen(_socketFd, SOMAXCONN) == -1)
	{
		// FAILED TO LISTEN ON SOCKET
		close(_socketFd);
		return (false);
	}
	return (true);
}

void	Server::Listen()
{
	std::cout << "Server is listening on " << _ip << ":" << _port << std::endl;

	int	poolResult;

	while (SERVER_RUNNING)
	{
		// Prepare client address structure to store incoming connection info
		struct sockaddr_in	client_addr;
		socklen_t			client_len = sizeof(client_addr);

		// Wait for incoming connections or timeout
		poolResult = poll(_pollFds.data(), _pollFds.size(), 1000); // 1000 ms timeout
		if (checkPoll(poolResult))
			break; // Exit if poll indicates an error or server is shutting down

		// Accept incoming client connection
		int	client_fd = accept(_socketFd, (struct sockaddr*)&client_addr, &client_len);
		
		// Check if accept() failed
		if (client_fd == -1)
		{
			continue;
		}

		// Handle the connected client
		handleClient(client_fd);
	}
}

// Check if the poll result indicates an error or if the server is still running
// false: Continue waiting for new connections
// true: Stop the server
bool	Server::checkPoll(int poolResult) const
{
	if (poolResult < 0)
	{
		std::cerr << "Poll error: " << strerror(errno) << std::endl;
		return (false); // Continue listening on error
	}
	else if (poolResult == 0) // Timeout occurred, no new connections
	{
		if (!SERVER_RUNNING)
		{
			std::cout << "Server shutting down..." << std::endl;
			return (true); // Stop the server
		}
	}

	return (false); // Continue listening for new connections
}