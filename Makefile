# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jwoo <jwoo@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/10 14:41:27 by jwoo              #+#    #+#              #
#    Updated: 2021/08/12 08:35:25 by jwoo             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

USER = $(LOGNAME)
NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
INCLUDEREADLINE = -I/Users/$(USER)/.brew/opt/readline/include
LIBREADLINE = -L/usr/include -L/Users/$(USER)/.brew/opt/readline/lib -lreadline
LIBMINISHELL = -L. -lminishell
LIBFT = -L./libft -lft
SOURCE = ft_split_space.c \
		ft_atoi_ret_error.c \
		ft_strjoin_with_free.c \
		ft_lstclear_two.c \
		ft_str_append.c \
		parse_lst_to_arr.c \
		parse_arg_to_lst.c \
		parse_file_type.c \
		parse_path.c \
		parse_exec.c \
		init_envp.c \
		export.c \
		signal_handler.c \
		mini_cd.c \
		mini_env.c \
		mini_exit.c \
		mini_export.c \
		mini_pwd.c \
		mini_unset.c \
		interpret.c \
		check_quote.c \
		remove_quote.c \
		replace_env.c \
		handle_line.c
OBJECT = $(SOURCE:.c=.o)
MAIN = minishell.c

all : $(NAME)

libft :
	@make -C ./libft all

$(NAME): $(LIB) $(MAIN) libft
#	gcc $(MAIN) $(CFLAGS) $(LIBMINISHELL) $(LIBFT) $(LIBREADLINE) -o $(NAME)
	gcc -g $(MAIN) $(CFLAGS) $(LIBMINISHELL) $(LIBFT) -L/usr/local/opt/readline/lib -lreadline -o $(NAME)
$(LIB): $(OBJECT)
	@ar rcs $(LIB) $(OBJECT)
$(OBJECT): $(SOURCE)
#	@gcc -c $(CFLAGS) $(SOURCE) $(INCLUDEREADLINE)
	@gcc -c $(CFLAGS) $(SOURCE) -I/usr/local/opt/readline/include

clean:
	@rm -f $(OBJECT)
	@make -C ./libft clean
fclean: clean
	@make -C ./libft fclean
	@rm -f $(LIB)
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re libft