NAME = ircserv

TESTFOLDER = test/bircd

CC = c++ -std=c++98 -g -Iincludes/
CC += -Wall -Wextra -Werror

SRCS = src/main.cpp \
	src/Channel/Channel.cpp \
	src/Channel/ChannelGettersSetters.cpp \
	src/Server/Server.cpp \
	src/Server/ServerListening.cpp \
	src/Server/HandleRequest.cpp \
	src/Server/Response.cpp \
	src/Server/ReadingFromClient.cpp \
	src/Server/Cmds/AddClient.cpp \
	src/Server/Cmds/RemoveClient.cpp \
	src/Server/Cmds/UserCmd.cpp \
	src/Server/Cmds/PassCmd.cpp \
	src/Utils.cpp \
	src/Client.cpp \

all: $(NAME)

$(NAME): $(SRCS)
	@$(MAKE) -s -C $(TESTFOLDER) all
	@echo "$(BLUE)[$(NAME)]:\t TESTER COMPILED$(RESET)"
	@$(CC) $(SRCS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)]:\t PROJECT COMPILED$(RESET)"

clean:
	@$(MAKE) -s -C $(TESTFOLDER) clean
	@echo "$(RED)[Tester]:\t CLEAN$(RESET)"
	@echo "$(RED)[$(NAME)]:\t CLEAN$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -s -C $(TESTFOLDER) fclean
	@echo "$(RED)[Tester]:\t FCLEAN$(RESET)"
	@echo "$(RED)[$(NAME)]:\t FCLEAN$(RESET)"

re: fclean all

PORT = 6667
PASSWORD = psw

arg = $(PORT) $(PASSWORD)

test: all
	./$(NAME) $(arg)


val: all
	valgrind ./$(NAME) $(arg)

#COLORS

GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
RESET=\033[0m