#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <csignal>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ReponseCode.hpp"

class	Channel;
class	Client;
class	Server;

int		parseInput(int argc, char **argv);

void	handleSigInt(int sig);

void	printRawMessage(int bytes_received, char *buffer);

#endif