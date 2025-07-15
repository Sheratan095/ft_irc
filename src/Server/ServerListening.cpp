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
	std::cout << "Client connected" << std::endl;

	char buffer[1024];
	ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
	
	if (bytes_received > 0)
	{
		buffer[bytes_received] = '\0';
		std::cout << "Received message: " << buffer << std::endl;
		
		send(client_fd, buffer, bytes_received, 0);
	}
	
	close(client_fd);
}