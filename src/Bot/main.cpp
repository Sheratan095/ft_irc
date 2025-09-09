#include "Bot.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <bot_port>" << std::endl;
		return 1;
	}
	
	std::string	server_ip = argv[1];
	int			server_port = safeAtoi(argv[2]);
	int			bot_port = safeAtoi(argv[3]);

	Bot bot(server_ip, server_port, bot_port);
	bot.run();
	
	return 0;
}