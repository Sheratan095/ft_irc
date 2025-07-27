#include "Response.hpp"

bool	sendResponse(int client_fd, ResponseCode code)
{
	std::string	response = composeResponse(code, getResponseByCode(code));

	ssize_t	bytesSent = send(client_fd, response.c_str(), response.size(), 0);

	if (bytesSent < 0)
	{
		std::cerr << "Error sending response to client: " << strerror(errno) << std::endl;
		return (false);
	}

	return (true);
}

bool	sendErrorResponse(int client_fd, ResponseCode code)
{
	std::string	errorMessage = getResponseByCode(code);

	if (errorMessage.empty())
	{
		std::cerr << "Error composing error response." << std::endl;
		return (false);
	}

	// Send the error message to the client
	ssize_t	bytesSent = send(client_fd, errorMessage.c_str(), errorMessage.size(), 0);

	if (bytesSent < 0)
	{
		std::cerr << "Error sending error response to client: " << strerror(errno) << std::endl;
		return (false);
	}

	return (true);
}

// Mostly used to send error during first connection
bool	sendErrorResponse(int client_fd, std::string reason)
{
	std::string	errorMessage = "ERROR :" + reason + "\r\n";

	if (errorMessage.empty())
	{
		std::cerr << "Error composing error response." << std::endl;
		return (false);
	}

	// Send the error message to the client

	ssize_t	bytesSent = send(client_fd, errorMessage.c_str(), errorMessage.size(), 0);

	if (bytesSent < 0)
	{
		std::cerr << "Error sending error response to client: " << strerror(errno) << std::endl;
		return (false);
	}

	return (true);
}


std::string	composeResponse(ResponseCode code, const std::string &message)
{
	std::ostringstream	oss;

	oss << ":server " << code << " : " << message << "\r\n";

	std::string	response = oss.str();


	return (response);
}

std::string	getResponseByCode(ResponseCode code)
{
	switch (code)
	{
		case RPL_WELCOME:
			return composeResponse(RPL_WELCOME, "Welcome to the IRC Network");
		case ERR_UNKNOWNCOMMAND:
			return composeResponse(ERR_UNKNOWNCOMMAND, "Unknown command");
		default:
			return composeResponse(code, "Unknown response code");
	}
}