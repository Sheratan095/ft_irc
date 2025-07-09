#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class	Server
{
	private:
		const unsigned short	port;
		const std::string		password;


	public:
		Server(const unsigned short port, const std::string &password);
		~Server();

		bool	start_server();

};

#endif