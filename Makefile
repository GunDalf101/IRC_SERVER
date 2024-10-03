CC = c++
CFLAGS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98 -g3 -fsanitize=address
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

HS   = ${shell ls *.hpp}

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) $(HS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	# make clean

%.o: %.cpp $(HS)
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run