
NAME = libft.a
SRC = \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c  ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
	ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
	ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
	ft_putendl_fd.c ft_putnbr_fd.c 

SRC_BONUS = \
	ft_lstnew.c ft_lstsize.c ft_lstadd_back.c \
	ft_lstadd_front.c ft_lstiter.c ft_lstlast.c \
	ft_lstdelone.c ft_lstclear.c \

OBJECTS = $(SRC:.c=.o)
OBJECTS_BONUS = $(SRC_BONUS:.c=.o)


CC = cc
CFLAGS = -Wall -Wextra -Werror

objects : $(OBJECTS) $(OBJECTS_BONUS)

all: $(NAME) 

$(NAME): $(OBJECTS) 
	ar rcs $@ $^
bonus: $(OBJECTS_BONUS)

$(OBJECTS_BONUS):${SRC_BONUS}
	${CC} ${CFLAGS} -c $^
	ar rcs $(NAME) $(OBJECTS_BONUS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^

clean:
	rm -f $(OBJECTS) $(OBJECTS_BONUS)
fclean: clean
	rm -f $(NAME)


re: fclean all

.PHONY: all clean  fclean re bonus
