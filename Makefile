CC = c++
CFLAGS = -fsanitize=address -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -g3
RM = rm -rf
NAME = ircserv

SRCS = webserv.cpp \
		Server.cpp \
		Client.cpp \
		Command.cpp \
		Channel.cpp \
		JOIN.cpp \
		TOPIC.cpp \
		INVITE.cpp \
		MODE.cpp \
		helper.cpp \
		BOT.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) webserv.hpp
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	# make clean

%.o: %.cpp
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run
