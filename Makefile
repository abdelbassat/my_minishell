

NAME = minishell


SRC =  ft_minishell.c ft_parce.c ft_env.c ft_unset.c ft_rediction.c ft_Wildcards.c ft_export.c ft_cd.c  ft_echo.c ft_command.c ft_pwd.c \
		ft_handel_quotes.c ft_heredoc.c ft_exit.c ft_pipe.c    ft_tree.c

OBJECTS = $(SRC:.c=.o)

CC = gcc

CFLAGS =    -lreadline -no-pie -Wall -Wextra -Werror  # -fsanitize=address 

all: $(NAME) 

	
$(NAME): $(OBJECTS) 
	@make -s -C libft
	@make bonus -s -C libft 
	@$(CC) 	$(SRC) $(CFLAGS) libft/libft.a -o $(NAME)
	@rm -rf *.o > /dev/null

%.o: %.c
	@$(CC)  -c $(CFLAGS) $^


clean:
	@make clean -s -C libft 
	@rm -f $(OBJECTS) $(OBJECTS_BONUS)
fclean: clean
	@make fclean -s -C libft  
	@rm -rf ./minishell

re: fclean all
run:all
	python3 -m c_formatter_42 *.c && clear && ./minishell
.PHONY: all clean  fclean re bonus

.SECONDARY: $(OBJECTS)
