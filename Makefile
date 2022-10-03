MODE		?=	Final
NAME		=	webserv
CXX			=	g++
CXXFLAGS 	=	-std=c++11 -pthread -Wall -Wextra -Werror -MMD -fPIC

INC_DIRS 	=	-Iinc \
				-I./srcs/Parser \
				-I./srcs/Server \
				-I./srcs/Utils \

SRC_DIR			= ./srcs/

SRCS_NAMES		=	main.cpp \
					Parser/Parser.cpp \
					Server/Server.cpp \

SRCS		= $(addprefix $(SRC_DIR), $(SRCS_NAMES))

ifeq ($(MODE), Debug)
	CXXFLAGS += -D_V_DEBUG_ -g -fsanitize=address
else
	CXXFLAGS += -O2 -fpie
endif

OBJS	= $(SRCS:.cpp=.o)
DEPS	= $(OBJS:.o=.d)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)
.PHONY:	all, clean, fclean, re
