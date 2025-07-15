#ifndef IRC_HPP
#define IRC_HPP

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
#include "Utils.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ReponseCode.hpp"

class	Channel;
class	Client;
class	Server;

#endif