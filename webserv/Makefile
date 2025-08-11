NAME = main
FLAGS = -Wall -Wextra -Werror -std=c++98
CPP = c++

SRC = niveau_1.cpp
OBJDIR = objs
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CPP) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "\033[32m$(NAME) created !\033[0m"
	
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@$(CPP) $(FLAGS) -c $< -o $@


clean:
	@rm -rf $(OBJDIR)
	@echo "\033[34mDeleting almost everything !\033[0m"
	
fclean: clean
	@rm -f $(NAME)
	@echo "\033[35mDeleting everything !\033[0m"
	
re: fclean all
	@echo "\033[33mmake re SUCCESS!\033[0m"

.PHONY: all clean fclean re