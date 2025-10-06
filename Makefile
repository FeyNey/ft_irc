NAME = irc

CC = c++
CFLAGS = -Wall -Wextra -Werror -g -std=c++98 -I include

SRCS_DIR = srcs
OBJS_DIR = .objs

LST_SRCS =	main.cpp \
			ListenSocketClass.cpp \
			RoomClass.cpp \
			ClientSocketClass.cpp \
			ASocketClass.cpp \
			ServerClass.cpp \
			RequestClass.cpp \
			ResponseClass.cpp \
			utils.cpp

LST_OBJS = $(LST_SRCS:.cpp=.o)


SRCS = $(addprefix $(SRCS_DIR)/, $(LST_SRCS))
OBJS = $(addprefix $(OBJS_DIR)/, $(LST_OBJS))

all : $(NAME)

clean :
	rm -rf $(OBJS_DIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p .objs
	$(CC) $(CFLAGS) -o $@ -c $<


$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

