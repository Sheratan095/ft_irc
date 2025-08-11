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

	ERR_PASSWDMISMATCH = 464,

	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NICKNAMEINUSE = 433,
	ERR_ERRONEUSNICKNAME = 432, // Invalid nickname

	ERR_NOSUCHCHANNEL = 403, // No such channel
	ERR_INVITEONLYCHAN = 473, // Cannot join channel (+i)
	ERR_CHANNELISFULL = 471, // Channel is full
	ERR_BADCHANNELKEY = 475, // Bad channel key

	RPL_NOTOPIC = 331, // No topic is set
	RPL_TOPIC = 332, // Topic of a channel
};

// Response format:
//:server.name <numeric> <target / *> <extra params> :<message>\r\n

void		sendResponse(Client *client, ResponseCode code, const std::string &params);

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params);

#endif