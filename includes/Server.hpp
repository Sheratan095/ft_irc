#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

extern bool SERVER_RUNNING;

class	Server
{
	private:
		const int			port;
		const std::string	password;
		int					socket_fd;

		bool	createSocket();
		bool	bindSocket();
		bool	startListening();
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