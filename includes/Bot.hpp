#ifndef BOT_HPP
#define BOT_HPP

#include "Irc.hpp"

class Client;

class Bot
{
	private:
		const std::string	_name;
		std::string			_ipAddress;
	
	public:
		Bot(const std::string &name);
		~Bot();

		std::string	getName() const;
		std::string	getPrefix() const;
		bool		isRegistered() const;

		void	setIpAddress(const std::string &ipAddress);

		void	sendWelcome(Client *client) const;
		void	reciveMessage(Client *client, const std::string &message) const;

};

#endif