CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = get_time.c monitoring.c  init.c  one_philo.c  philo.c clean.c
OBJS = get_time.o monitoring.o  init.o  one_philo.o  philo.o clean.o

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c -pthread $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

aclean: all clean

.PHONY: all clean fclean re
