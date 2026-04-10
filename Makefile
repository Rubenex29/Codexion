NAME = codexion

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = codexion.c \
	parser.c \
	setup.c \
	utils.c \
	routine.c \
	monitor.c \
	actions.c \
	def_mutex.c \
	fifo.c

OBJS = $(SRCS:.c=.o)
HEADER = codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
