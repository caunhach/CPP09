NAME = btc
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = main.cpp BitcoinExchange.cpp
OBJS = $(SRCS:.cpp=.o)
INCS = BitcoinExchange.hpp
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean