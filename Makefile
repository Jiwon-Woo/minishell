USER = jwoo
NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
LIBREADLINE = -L/usr/include -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -I /Users/$(USER)/.brew/opt/readline/include
LIBMINISHELL = -L. -lminishell
LIBFT = -L./libft -lft
SOURCE = ft_split_space.c \
		ft_atoi_ret_error.c \
		ft_arg_arr.c
OBJECT = $(SOURCE:.c=.o)
MAIN = minishell.c
# MAIN = main.c

TEMP = -L/usr/local/opt/ruby/lib -I/usr/local/opt/ruby/include

all : $(NAME)

libft :
	@make -C ./libft all

$(NAME): $(LIB) $(MAIN) libft
	gcc $(MAIN) $(LIBMINISHELL) $(LIBFT) $(LIBREADLINE) -o $(NAME)
#	gcc $(MAIN) $(LIBMINISHELL) $(LIBFT) -L/usr/include -lreadline $(TEMP) -o $(NAME)
$(LIB): $(OBJECT)
	@ar rcs $(LIB) $(OBJECT)
$(OBJECT): $(SOURCE)
	@gcc -c $(SOURCE)

clean:
	@rm -f $(OBJECT)
	@make -C ./libft clean
fclean: clean
	@make -C ./libft fclean
	@rm -f $(LIB)
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re libft