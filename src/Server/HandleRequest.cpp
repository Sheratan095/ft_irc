#include "Server.hpp"

void	Server::handleRequest(int client_fd)
{
	std::vector<IRCMessage>	messages;
	std::string				message = readMessageFromClient(client_fd);
	if (!message.empty())
	{
		messages = parseMessage(message);
		printRawMessage(messages);
	}

	// std::cout << std::endl << "Client connected" << std::endl;

	// // Keep connection open and handle multiple messages
	
	// close(client_fd);
	// std::cout << "Client connection closed" << std::endl;
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