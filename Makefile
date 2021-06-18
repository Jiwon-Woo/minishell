USER = jwoo
NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
LIBREADLINE = -L/usr/include -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -I /Users/$(USER)/.brew/opt/readline/include
LIBMINISHELL = -L. -lminishell
LIBFT = -L./libft -lft
SOURCE = ft_split_space.c \
		ft_atoi_ret_error.c \
		echo_exception.c
OBJECT = $(SOURCE:.c=.o)
MAIN = main.c

all : $(NAME)

# libft : ./libft/libft.a
# 	make -C ./libft all

$(NAME): $(LIB) $(MAIN)
	make -C ./libft all
	gcc $(MAIN) $(CFLAGS) $(LIBMINISHELL) $(LIBFT) $(LIBREADLINE) -o $(NAME)
$(LIB): $(OBJECT)
	@ar rcs $(LIB) $(OBJECT)
$(OBJECT): $(SOURCE)
	@gcc $(CFLAGS) -c $(SOURCE)

clean:
	@rm -f $(OBJECT)
	@make -C ./libft clean
fclean: clean
	@make -C ./libft fclean
	@rm -f $(LIB)
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re