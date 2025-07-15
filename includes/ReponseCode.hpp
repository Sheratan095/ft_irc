#ifndef REPONSECODE_HPP
#define REPONSECODE_HPP

#include "Irc.hpp" 

enum	ResponseCode
{
	RPL_WELCOME = 001
};

std::string	getResponseMessage(ResponseCode code);

std::string	composeResponse(ResponseCode code, const std::string &message);

std::string	getResponseByCode(ResponseCode code);

#endif