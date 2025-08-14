NAME = main
FLAGS = -Wall -Wextra -Werror -std=c++98
CPP = c++

SRC_1 = level_1.cpp
SRC_2 = level_2.cpp
SRC_3 = level_3.cpp
SRC_4 = level_4.cpp
SRC_5 = level_5.cpp
SRC_6 = level_6.cpp
SRC_7 = level_7.cpp
SRC_8 = level_8.cpp
SRC_9 = level_9.cpp
SRC_10 = level_10.cpp
OBJDIR = objs
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CPP) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "\033[32m$(NAME) created !\033[0m"
	
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@$(CPP) $(FLAGS) -c $< -o $@

lvl1:
	@$(CPP) $(FLAGS) $(SRC_1) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 1 !\033[0m"

lvl2:
	@$(CPP) $(FLAGS) $(SRC_2) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 2 !\033[0m"

lvl3:
	@$(CPP) $(FLAGS) $(SRC_3) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 3 !\033[0m"

lvl4:
	@$(CPP) $(FLAGS) $(SRC_4) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 4 !\033[0m"

lvl5:
	@$(CPP) $(FLAGS) $(SRC_5) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 5 !\033[0m"

lvl6:
	@$(CPP) $(FLAGS) $(SRC_6) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 6 !\033[0m"

lvl7:
	@$(CPP) $(FLAGS) $(SRC_7) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 7 !\033[0m"

lvl8:
	@$(CPP) $(FLAGS) $(SRC_8) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 8 !\033[0m"

lvl9:
	@$(CPP) $(FLAGS) $(SRC_9) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 9 !\033[0m"

lvl10:
	@$(CPP) $(FLAGS) $(SRC_10) -o $(NAME)
	@echo "\033[32m$(NAME) created for niveau 10 !\033[0m"

clean:
	@rm -rf $(OBJDIR)
	@echo "\033[34mDeleting almost everything !\033[0m"
	
fclean: clean
	@rm -f $(NAME)
	@echo "\033[35mDeleting everything !\033[0m"
	
re: fclean all
	@echo "\033[33mmake re SUCCESS!\033[0m"

.PHONY: all clean fclean re