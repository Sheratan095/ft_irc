#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <csignal>
#include <cerrno>
#include "Server.hpp"
#include "ReponseCode.hpp"

int		parseInput(int argc, char **argv);

void	handleSigInt(int sig);


#endif