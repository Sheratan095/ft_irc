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

		bool	switchCommand(const IRCMessage &message, Client *client);


		std::string				readMessageFromClient(int client_fd) const;
		std::vector<IRCMessage>	parseMessage(const std::string &message) const;
		void					printRawMessage(const std::vector<IRCMessage> &messages) const;
		Client*					findClientByName(const std::string &nickname) const;


		// commands
		bool	addClient(pollfd clientPollFd, const std::string &ip_str); // first connection
		void	quitCmd(Client *client, const IRCMessage &message);
		void	quitCmd(Client *client, const std::string &reason); // used for like end task or in general when the client process isn't disconnected in normal way
		void	userCmd(Client *client, const IRCMessage &message);
		void	passCmd(Client *client, const IRCMessage &message);
		void	nickCmd(Client *client, const IRCMessage &message);
		void	joinCmd(Client *client, const IRCMessage &message);
		void	modeCmd(Client *client, const IRCMessage &message);
		void	whoCmd(Client *client, const IRCMessage &message);
		void	topicCmd(Client *client, const IRCMessage &message);




		void	notifyNickChange(Client *sender, const std::string &oldNickname) const;
		void	notifyQuit(Client *sender, const std::string &reason) const;
		void	notifyJoin(Client *client, Channel *channel) const;
		void	NotifyTopicChange(Client *client, Channel *channel) const;


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