#include "Server.hpp"

void	Server::run()
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

		// Loop through the poll file descriptors
		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN) // Check if the socket is ready for reading
			{
				if (_pollFds[i].fd == _socketFd) // If it's the server socket
					handleConnectionRequest(client_addr, client_len); // Handle new client connection
				else // If it's a client socket
					handleClient(_pollFds[i].fd); // Handle existing client communication
			}
		}
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

void	Server::handleConnectionRequest(struct sockaddr_in	client_addr, socklen_t client_len)
{
	// Accept incoming client connection
	int	client_fd = accept(_socketFd, (struct sockaddr*)&client_addr, &client_len);
	
	if (client_fd == -1)
		std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;

	// Add new client to the poll list
	pollfd	clientPollFd = {};
	clientPollFd.fd = client_fd;
	clientPollFd.events = POLLIN;
	_pollFds.push_back(clientPollFd);

	handleClient(client_fd); // Handle the connected client
}