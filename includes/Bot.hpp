#ifndef BOT_HPP
#define BOT_HPP

#include "Irc.hpp"

class Client;
struct IRCMessage;

class Bot
{
	private:
		const std::string			_name;
		std::string					_ipAddress;
		std::vector<std::string>	_responses;
	
	public:
		Bot(const std::string &name);
		~Bot();

		std::string	getName() const;
		std::string	getPrefix() const;
		bool		isRegistered() const;

		void	setIpAddress(const std::string &ipAddress);

		void	sendWelcome(Client *client) const;
		void	receiveMessage(Client *client, const IRCMessage &message) const;

};

#endif