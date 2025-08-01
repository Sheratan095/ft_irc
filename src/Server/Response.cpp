#include "Response.hpp"

bool sendResponse(const Client &client, ResponseCode code, const std::string &params)
{
	std::string	response = composeResponse(code, client.getNickname(), params);

	ssize_t	bytesSent = send(client.getSocketFd(), response.c_str(), response.size(), 0);

	if (bytesSent < 0)
	{
		std::cerr << "Error sending response to client fd: " << client.getSocketFd() << std::endl;
		return false;
	}

	return true;
}

bool	sendCustomResponse(const int client_fd, const std::string &response)
{
	ssize_t	bytesSent = send(client_fd, response.c_str(), response.size(), 0);

	if (bytesSent < 0)
	{
		std::cerr << "Error sending custom response to client fd: " << client_fd << std::endl;
		return (false);
	}

	return (true);
}

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params)
{
	std::ostringstream	oss;

	oss << ":" SERVER_NAME
		<< " " << code
		<< " " << (targetName.empty() ? "*" : targetName)
		<< " " << params
		<< "\r\n";

	return (oss.str());
}

std::string	getResponseByCode(ResponseCode code)
{
	switch (code)
	{
		case RPL_WELCOME:
			return ("Welcome to the IRC Network");
		case ERR_SRVFULL:
			return ("Server is full");
		case ERR_UNKNOWNCOMMAND:
			return ("Unknown command");
		default:
			return ("Unknown response code");
	}
}