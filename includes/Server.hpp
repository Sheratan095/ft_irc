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
		SocketFd						_socketFd;

		std::map<SocketFd, Client*>		_clients;
		std::vector<pollfd>				_pollFds;

		std::map<std::string, Channel*>	_channels;

		bool	createSocket();
		bool	bindSocket();
		bool	startSocketListening();

		void	run();
		bool	checkPoll(int poolResult) const;

		void	handleConnectionRequest(struct sockaddr_in	client_addr, socklen_t client_len);
		void	handleDisconnection(int client_fd);
		void	handleRequest(int client_fd);

		char	*readMessageFromClient(int client_fd);
		void	printRawMessage(int bytes_received, char *buffer);


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