#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Irc.hpp"

class Client; // Forward declaration

//IRC numeric replies must always be 3 digits.
enum	ResponseCode
{
	RPL_WELCOME = 001,

	ERR_SRVFULL = 463,
	ERR_UNKNOWNCOMMAND = 421,

	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
};

// Response format:
//:server.name <numeric> <target / *> <extra params> :<message>\r\n

bool	sendResponse(const Client &client, ResponseCode code, const std::string &params);

bool	sendCustomResponse(const int client_fd, const std::string &response);

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params);

// bool	sendCustomErrorResponse(int client_fd, std::string customError);

std::string	getResponseByCode(ResponseCode code);

#endif