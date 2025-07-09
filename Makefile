NAME = ircserv

CC = c++ -std=c++98 -g -Iincludes/
# CC += -Wall -Wextra -Werror

SRCS = src/main.cpp src/Server.cpp

all: $(NAME)

$(NAME): $(SRCS)
	@$(CC) $(SRCS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)]:\t PROJECT COMPILED$(RESET)"

clean:
	@echo "$(RED)[$(NAME)]:\t CLEAN$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)[$(NAME)]:\t FCLEAN$(RESET)"

re: fclean all

PORT = 6667
PASSWORD = password

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