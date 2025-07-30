#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Irc.hpp" 

enum	ResponseCode
{
	RPL_WELCOME = 001,

	ERR_UNKNOWNCOMMAND = 4216995
};

bool	sendResponse(int client_fd, ResponseCode code);

bool	sendErrorResponse(int client_fd, ResponseCode code);

bool	sendErrorResponse(int client_fd, ResponseCode code, std::string targetName);

bool	sendErrorResponse(int client_fd, std::string reason);

std::string	composeResponse(ResponseCode code, const std::string &message, const std::string &targetName);

std::string	getResponseByCode(ResponseCode code, const std::string &targetName);

#endif