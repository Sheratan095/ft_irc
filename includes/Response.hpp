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

	ERR_NOTREGISTERED = 451,

	ERR_PASSWDMISMATCH = 464,

	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NICKNAMEINUSE = 433,
	ERR_ERRONEUSNICKNAME = 432, // Invalid nickname

	ERR_NOSUCHCHANNEL = 403, // No such channel
	ERR_INVITEONLYCHAN = 473, // Cannot join channel (+i)
	ERR_CHANNELISFULL = 471, // Channel is full
	ERR_BADCHANNELKEY = 475, // Bad channel key

	ERR_NOTONCHANNEL = 442,

	RPL_NOTOPIC = 331, // No topic is set
	RPL_TOPIC = 332, // Topic of a channel

	RPL_WHOREPLY = 352, // Reply to WHO command
	RPL_ENDOFWHO = 315, // End of WHO list

	RPL_NAMREPLY = 353, // Reply to NAMES command
	RPL_ENDOFNAMES = 366, // End of NAMES list

	RPL_CHANNELMODEIS = 324,

	ERR_NOSUCHNICK = 401, // Nick not found
	ERR_USERONCHANNEL = 443, // User is already on channel
	ERR_CHANOPRIVSNEEDED = 482, // Channel privileges needed
	RPL_INVITE = 314,
};

// Response format:
//:server.name <numeric> <target / *> <extra params> :<message>\r\n

void		sendResponse(Client *client, ResponseCode code, const std::string &params);

std::string	composeResponse(ResponseCode code, const std::string &targetName, const std::string &params);

void		sendSrvFULLError(int client_fd);

#endif