NAME = ircserv

TESTFOLDER = test/bircd

CC = c++ -std=c++98 -Ofast -g
CC += -Wall -Wextra -Werror

SRCS = src/main.cpp \
	src/Channel/Channel.cpp \
	src/Channel/ChannelGettersSetters.cpp \
	src/Channel/ModeProperties.cpp \
	src/Server/Server.cpp \
	src/Server/ServerUtils.cpp \
	src/Server/ServerListening.cpp \
	src/Server/HandleRequest.cpp \
	src/Server/Response.cpp \
	src/Server/ReadingFromClient.cpp \
	src/Server/GlobalNotifications.cpp \
	src/Server/ClientManagement.cpp \
	src/Server/Cmds/QuitCmd.cpp \
	src/Server/Cmds/UserCmd.cpp \
	src/Server/Cmds/PassCmd.cpp \
	src/Server/Cmds/NickCmd.cpp \
	src/Server/Cmds/JoinCmd.cpp \
	src/Server/Cmds/WhoCmd.cpp \
	src/Server/Cmds/ModeCmd.cpp \
	src/Server/Cmds/TopicCmd.cpp \
	src/Server/Cmds/InviteCmd.cpp \
	src/Server/Cmds/KickCmd.cpp \
	src/Server/Cmds/PrivmsgCmd.cpp \
	src/Server/Cmds/WhoisCmd.cpp \
	src/Server/Cmds/CapCmd.cpp \
	src/Server/Cmds/ListCmd.cpp \
	src/Server/Cmds/ImBotCmd.cpp \
	src/Client/Client.cpp \
	src/Utils.cpp \

BOT_NAME = MagicBall
BOT_SRCS = Bot/src/Bot.cpp \
		Bot/src/main.cpp \
		Bot/src/BotUtils.cpp \
		Bot/src/BotConnection.cpp \


all: $(NAME) $(BOT_NAME)

$(NAME): $(SRCS)
	@$(CC) $(SRCS) -Iincludes/ -o $(NAME)
	@echo "$(GREEN)[$(NAME)]:\t PROJECT COMPILED$(RESET)"

$(BOT_NAME): $(BOT_SRCS)
	@$(CC) -IBot/includes/ $(BOT_SRCS) -o $(BOT_NAME)
	@echo "$(GREEN)[$(NAME)]:\t BOT COMPILED$(RESET)"

clean:
	@echo "$(RED)[$(NAME)]:\t CLEAN$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(BOT_NAME)
	@echo "$(RED)[$(NAME)]:\t FCLEAN$(RESET)"

re: fclean all

PORT = 6667
PASSWORD = psw

arg = $(PORT) $(PASSWORD)

test: all
	./$(NAME) $(arg)

test_bot: all
	./$(BOT_NAME) 127.0.1.1 $(PORT) $(PASSWORD)

val: all
	valgrind ./$(NAME) $(arg)

val_bot: all
	valgrind ./$(BOT_NAME) 127.0.1.1 $(PORT) $(PASSWORD)


#COLORS

GREEN=\e[0;32m
RED=\e[0;91m
BLUE=\e[0;34m
RESET=\e[0m