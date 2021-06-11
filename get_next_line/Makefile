NAME = get_next_line.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -c

FILES =	get_next_line.c \
			get_next_line_utils.c

OBJS = $(FILES:%.c=%.o)

all : $(NAME)

$(OBJS) : $(FILES)
	$(CC) $(CFLAGS) $(FILES)

$(NAME) : $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re
