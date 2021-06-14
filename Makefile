NAME = minishell
LIB = libminishell.a
CFLAGS = -Wall -Wextra -Werror
LFLAG = -L. -lminishell
SOURCE = ft_gnl.c \
		ft_split_space.c \
		echo_exception.c
OBJECT = $(SOURCE:.c=.o)
MAIN = main.c

all : $(NAME)

$(NAME): $(LIB) $(MAIN)
	gcc $(MAIN) $(CFLAGS) $(LFLAG) -o $(NAME)
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