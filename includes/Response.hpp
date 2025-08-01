#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Irc.hpp"

class Client; // Forward declaration

//IRC numeric replies must always be 3 digits.
enum	ResponseCode
{
	RPL_WELCOME = 001,

	ERR_SRVFULL = 463,
	ERR_UNKNOWNCOMMAND = 421
};

// Response format:
//:server.name <numeric> <target / *> <extra params> :<message>\r\n

bool	sendResponse(const Client &client, ResponseCode code, const std::vector<std::string> &params);

bool	sendErrorResponse(int client_fd, ResponseCode code, const std::string &targetName = "", const std::string &extraParams = "");

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::vector<std::string> &params);

// bool	sendCustomErrorResponse(int client_fd, std::string customError);

std::string	getResponseByCode(ResponseCode code);

#endif