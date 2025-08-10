#include "Response.hpp"

void sendResponse(const Client &client, ResponseCode code, const std::string &params)
{
	std::string	response = composeResponse(code, client.getNickname(), params);

	sendMessage(client.getSocketFd(), response);
}


