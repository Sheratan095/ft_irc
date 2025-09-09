#ifndef BOT_HPP
#define BOT_HPP

#include "Irc.hpp"

class Client;

class Bot
{
	private:
		const std::string	_name;
	
	public:
		Bot(const std::string &name);
		~Bot();

		std::string	getName() const;

		void	reciveMessage(Client *client, const std::string &message) const;

};

#endif