CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
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

HS   = BOT.hpp Channel.hpp Client.hpp Command.hpp CommandFactory.hpp CommandRpl.hpp Server.hpp webserv.hpp

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