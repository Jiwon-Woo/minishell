CC = gcc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a
SOURCE = ft_strlen.c \
	ft_strncmp.c \
	ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_isspace.c \
	ft_strdup.c \
	ft_strjoin.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c \
	ft_lstnew.c
OBJECT = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME): $(OBJECT)
	@ar	rcs $(NAME) $(OBJECT)
$(OBJECT): $(SOURCE)
	@$(CC) $(CFLAGS) -c $(SOURCE)

clean: 
	@rm -f $(OBJECT)
fclean: clean
	@rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re