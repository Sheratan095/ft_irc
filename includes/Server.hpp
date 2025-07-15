#ifndef SERVER_HPP
#define SERVER_HPP

#include "Irc.hpp"

// Forward declarations
class	Channel;
class	Client;

extern bool SERVER_RUNNING;

class	Server
{
	private:
		const int						_port;
		std::string						_ip;
		const std::string				_password;
		int								_socket_fd;
		std::map<std::string, Channel*>	_channels; // Use pointer instead

		bool	createSocket();
		bool	bindSocket();
		bool	startListening();
		void	Listen();
		void	handleClient(int client_fd);

	public:
		Server(const int port, const std::string &password);
		~Server();

		void	startServer();

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

#endif