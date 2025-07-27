#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Irc.hpp" 

enum	ResponseCode
{
	RPL_WELCOME = 001,

	ERR_UNKNOWNCOMMAND = 421
};

bool	sendResponse(int client_fd, ResponseCode code);

bool	sendErrorResponse(int client_fd, ResponseCode code);

bool	sendErrorResponse(int client_fd, std::string reason);

std::string	composeResponse(ResponseCode code, const std::string &message);

std::string	getResponseByCode(ResponseCode code);

#endif