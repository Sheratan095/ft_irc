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

// bool	sendCustomResponse(const int client_fd, const std::string &response)
// {
// 	ssize_t	bytesSent = send(client_fd, response.c_str(), response.size(), 0);

// 	if (bytesSent < 0)
// 	{
// 		std::cerr << "Error sending custom response to client fd: " << client_fd << std::endl;
// 		return (false);
// 	}

// 	return (true);
// }

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params)
{
	std::ostringstream	oss;
	oss << ":" << SERVER_NAME << " " << code << " " << (targetName.empty() ? "*" : targetName);

	switch (code)
	{
		case RPL_WELCOME:
			oss << " :Welcome to the IRC Network";
			break;

		case ERR_SRVFULL:
			oss << " :Server is full";
			break;

		case ERR_UNKNOWNCOMMAND:
			oss << " " << params << " :Unknown command";
			break;

		case ERR_PASSWDMISMATCH:
			oss << " :Password incorrect";
			break;

		default:
			oss << " :Unknown response code";
			break;
	}

	oss << "\r\n";

	return (oss.str());
}
