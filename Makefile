USER = $(LOGNAME)
NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
LIBREADLINE = -L/usr/include -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -I /Users/$(USER)/.brew/opt/readline/include
LIBMINISHELL = -L. -lminishell
LIBFT = -L./libft -lft
SOURCE = ft_split_space.c \
		ft_atoi_ret_error.c \
		ft_strjoin_with_free.c \
		ft_lstclear_two.c \
		ft_str_append.c \
		ft_free_null.c \
		parse_lst_to_arr.c \
		parse_arg_to_lst.c \
		env.c \
		export.c \
		signal_handler.c \
		mini_cmd.c \
		parse_path.c \
		interpret.c \
		check_quote.c \
		remove_quote.c \
		replace_env.c
OBJECT = $(SOURCE:.c=.o)
MAIN = minishell.c

LOCAL = -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline

all : $(NAME)

libft :
	@make -C ./libft all

$(NAME): $(LIB) $(MAIN) libft
#	gcc $(MAIN) $(CFLAGS) $(LIBMINISHELL) $(LIBFT) $(LIBREADLINE) -o $(NAME)
	gcc $(MAIN) $(CFLAGS) $(LIBMINISHELL) $(LIBFT) $(LOCAL) -o $(NAME)
$(LIB): $(OBJECT)
	@ar rcs $(LIB) $(OBJECT)
$(OBJECT): $(SOURCE)
	@gcc -c $(CFLAGS) $(SOURCE)

clean:
	@rm -f $(OBJECT)
	@make -C ./libft clean
fclean: clean
	@make -C ./libft fclean
	@rm -f $(LIB)
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re libft