NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
READLINE = -L/usr/include -lreadline
LIBFLAG = -L. -lminishell
SOURCE = ft_gnl.c \
		ft_split_space.c \
		ft_atoi.c \
		ft_strjoin.c \
		echo_exception.c
OBJECT = $(SOURCE:.c=.o)
MAIN = main.c

all : $(NAME)

$(NAME): $(LIB) $(MAIN)
	gcc $(MAIN) $(CFLAGS) $(LIBFLAG) $(READLINE) -o $(NAME)
$(LIB): $(OBJECT)
	ar rcs $(LIB) $(OBJECT)
$(OBJECT): $(SOURCE)
	gcc $(CFLAGS) -c $(SOURCE)

clean:
	@rm -f $(OBJECT)
fclean: clean
	@rm -f $(LIB)
	@rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re