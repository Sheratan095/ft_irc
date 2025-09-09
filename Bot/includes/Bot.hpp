#ifndef BOT_HPP
#define BOT_HPP

#include "BotUtils.hpp"

extern bool BOT_RUNNING;


class Bot
{
	private:
		std::string	_server_ip;
		int			_server_port;
		SocketFd	_socketFd;

		std::string	_nick;

		std::string	_ip;

		std::vector<std::string>	_responses;


	public:
		Bot(const std::string &server_ip, int server_port);
		~Bot();

		bool	createSocket();
		void	run();
		void	connectToServer(const std::string &password);

		std::vector<IRCMessage>	parseMessage(const std::string &message) const;

		void	privmsgCmd(const IRCMessage &message) const;


	//-------------------------EXCEPTIONS-------------------------

		class SocketException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: socket creation failed";
			}
		};

		class BindException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: bind failed";
			}
		};

		class ListenException : public std::exception
		{
			virtual const char *what() const throw()
			{
				return "Error: listen failed";
			}
		};


};

#endif // BOT_HPP