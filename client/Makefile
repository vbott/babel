NAME		=  net

DIR		=  .

SRCS		= InetAddr.cpp	\
				Error.cpp			\
				Socket.cpp			\
				SocketIO.cpp		\
				SocketAcceptor.cpp	\
				SocketConnector.cpp	\
				SocketStream.cpp	\
				SetupNetwork.cpp	\
				NetHandler.cpp		\
				Reactor.cpp			\
				PollPolicy.cpp		\
				EpollPolicy.cpp		\
				IOVec.cpp			\
				main.cpp

OBJS		= $(SRCS:.cpp=.o)

CC		= g++

RM		= rm -f

CXXFLAGS	= -W -Wall -Wextra -g3 -pipe

LDFLAGS		= -lportaudio 

$(NAME)		: $(OBJS)
		$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

all		: $(NAME)

clean		:
		$(RM) $(OBJS)

fclean		: clean
		$(RM) $(NAME)

re		: fclean all

