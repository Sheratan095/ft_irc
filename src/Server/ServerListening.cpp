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

		// Check if the server socket is ready for reading => new connection
		if (_pollFds[0].revents & POLLIN)
			handleConnectionRequest(client_addr, client_len);

		for (size_t i = 1; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN) // Check if client socket is ready for reading
				handleRequest(_pollFds[i].fd);
			else if (_pollFds[i].revents & POLLHUP) // Check if client disconnected
				handleDisconnection(_pollFds[i].fd);
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

// TO DO
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

	std::vector<IRCMessage>	messages;
	std::string	message = readMessageFromClient(client_fd);
	if (!message.empty())
	{
		messages = parseMessage(message);
		if (!messages.empty())
			printRawMessage(messages); // Print the parsed messages to console
		// printRawMessage(messages);
	}

	// handleClient(client_fd); // Handle the connected client
}

// TO DO
void	Server::handleDisconnection(int client_fd)
{
	std::cout << "Client with fd: " << client_fd << " disconnected" << std::endl;

	close(client_fd); // Close the client socket

	for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if (it->fd == client_fd)
		{
			_pollFds.erase(it);
			break;
		}
	}
}