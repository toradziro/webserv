MODE		?=	Final
NAME		=	webserv
CXX			=	g++
CXXFLAGS 	=	-std=c++11 -pthread -Wall -Wextra -Werror -MMD -fPIC

INC_DIRS 	=	-Iinc \
				-I./srcs/Parser \
				-I./srcs/Utils \
				-I./srcs/Core/Request \
				-I./srcs/Core/Server \
				-I./srcs/Core/Response \

SRC_DIR			=	./srcs/

SRCS_NAMES		=	main.cpp \
					Parser/Parser.cpp \
					Parser/Lexem.cpp \
					Parser/LexemsCollection.cpp \
					Parser/ContentTypeParser.cpp \
					Parser/ContentTypeCollection.cpp \
					Parser/ConfigError.cpp \
					Core/Server/ServerCreator.cpp \
					Core/Server/Locations.cpp \
					Core/Server/Server.cpp \
					Core/Server/Selector.cpp \
					Core/Response/Response.cpp \
					Core/Request/Request.cpp \
					Core/Request/RequestCollection.cpp \
					Core/Request/RequestFabric.cpp \

SRCS		= $(addprefix $(SRC_DIR), $(SRCS_NAMES))

NUM_THREADS	= $(shell nproc)
CXXFLAGS	+= -D_NUM_THREADS=$(NUM_THREADS)

ifeq ($(MODE), debug)
	CXXFLAGS += -D_DEBUG -g -fsanitize=address
else
	CXXFLAGS += -O2
endif

OBJS	= $(SRCS:.cpp=.o)
DEPS	= $(OBJS:.o=.d)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)
.PHONY:	all, clean, fclean, re
