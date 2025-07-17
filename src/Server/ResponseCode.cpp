#include "ReponseCode.hpp"
#include <sstream>

std::string	getResponseMessage(ResponseCode code)
{
	switch (code)
	{
		case RPL_WELCOME:
			return "Welcome to the IRC Network";
		default:
			return "Unknown response code";
	}
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
			return composeResponse(RPL_WELCOME, getResponseMessage(RPL_WELCOME));
		default:
			return composeResponse(code, "Unknown response code");
	}
}