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
					Core/Request/RequestCollection.cpp \
					Core/Response/Response.cpp \
					Core/Response/ResponseGET.cpp \
					Core/Request/Request.cpp \
					Core/Request/RequestFabric.cpp \

SRCS		= $(addprefix $(SRC_DIR), $(SRCS_NAMES))

NUM_THREADS	= $(shell nproc)
CXXFLAGS	+= -D_NUM_THREADS=$(NUM_THREADS)

ifeq ($(MODE), Debug)
	CXXFLAGS += -D_DEBUG -g -fsanitize=address
#-fsanitize=undefined
else
	CXXFLAGS += -O2 -march=native
endif

ifeq ($(MODE), MemoryCheck)
	CYCLE_COUNT ?= 6
	CXXFLAGS += -D_MEMORY_PROFILE -D_CICLE_COUNT=$(CYCLE_COUNT) -g
endif

OBJS	= $(SRCS:.cpp=.o)
DEPS	= $(OBJS:.o=.d)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INC_DIRS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

test:
	bash ./utilites/run_tests.sh

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

fclean: clean
	@rm -f $(NAME)
	@rm -f server.log
	@rm -f valgrind.log
	@rm -f speed_test.log
	@rm -f utilites/autotest/autotest

re: fclean all

-include $(DEPS)
.PHONY:	all, clean, fclean, re
