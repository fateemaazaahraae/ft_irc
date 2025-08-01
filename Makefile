NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ./srcs/main.cpp ./srcs/Server.cpp ./srcs/Client.cpp ./srcs/Server_tools.cpp ./srcs/cmds/pass.cpp ./srcs/cmds/nick.cpp ./srcs/cmds/user.cpp
OBJ = $(SRC:%.cpp=%.o)

# Color codes
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)🔧 Preparing the executable...$(NC)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)The executable '$(NAME)' is ready!$(NC)"

clean:
	@echo "$(RED)🧹 Cleaning object files...$(NC)"
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	@echo "$(GREEN)✅ Full clean complete.$(NC)"

re: fclean all

.PHONY: all clean fclean re
