#include "Server.hpp"

void	Server::run()
{
	std::cout << "Server is listening on " << _ip << ":" << _port << std::endl << std::endl;

	int	poolResult;

	while (SERVER_RUNNING)
	{
		// Prepare client address structure to store incoming connection info
		struct sockaddr_in	client_addr;
		socklen_t			client_len = sizeof(client_addr);

		// Wait for incoming connections or timeout (blocking call)
		poolResult = poll(_pollFds.data(), _pollFds.size(), 1000); // 1000 ms timeout
		if (checkPoll(poolResult))
			break; // Exit if poll indicates an error or server is shutting down

		// Check if the server socket is ready for reading => new connection
		if (_pollFds[0].revents & POLLIN)
			handleConnectionRequest(client_addr, client_len);

		for (size_t i = 1; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN) // Check if client socket is ready for reading
				handleRequest(_pollFds[i].fd);
			else if (_pollFds[i].revents & POLLHUP) // Check if client disconnected
				quitCmd(_clients[_pollFds[i].fd], "Client disconnected");
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
		if (errno == EINTR)
		{
			std::cerr << "Poll interrupted by signal" << std::endl;
			return (false); // Continue listening
		}
		else
		{
			std::cerr << "Poll error: " << strerror(errno) << std::endl;
			return (true); // Stop the server on error
		}
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
	{
		std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
		return; // Handle error, but continue accepting new connections
	}

	// Set the client socket to non-blocking mode
	// I don't override the existing flags, just add O_NONBLOCK
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(client_fd);
		return ;
	}

	//Create a pollfd structure for the new client
	pollfd	clientPollFd = {};
	clientPollFd.fd = client_fd;
	clientPollFd.events = POLLIN;

	std::cout << "New connection incoming with fd: " << client_fd << std::endl;

	// Get the IP address as a string
	std::string	ip_str = inet_ntoa(client_addr.sin_addr);

	// Try to add the new client to the server's client list
	if (addClient(clientPollFd, ip_str) == false)
	{
		std::cerr << "Failed to add client with fd: " << client_fd << std::endl;

		sendSrvFULLError(client_fd);

		close(client_fd);

		return;
	}

	// Successfully added the client, now handle the connection	

	std::cout << "Connection accepted, new client created with fd: " << client_fd << std::endl << std::endl;

	// Handle the initial request from the new client (the welcome message is sent when the client is FULL registered)
	// handleRequest(client_fd);
}
