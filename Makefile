MODE		?=	Final
NAME		=	webserv
CXX			=	g++
CXXFLAGS 	=	-std=c++17 -pthread -Wall -Wextra -Werror -MMD -fPIC

INC_DIRS 	=	-Iinc \
				-I./srcs/Parser \
				-I./srcs/Utils \
				-I./srcs/Server \
				-I./srcs/Server/RequestProcessor \
				-I./srcs/Server/RequestProcessor/RequestHandler \

SRC_DIR			=	./srcs/

SRCS_NAMES		=	main.cpp \
					Utils/SecureMemset.cpp \
					Parser/Parser.cpp \
					Parser/Lexem.cpp \
					Parser/LexemsCollection.cpp \
					Parser/ContentTypeParser.cpp \
					Parser/ContentTypeCollection.cpp \
					Parser/ConfigError.cpp \
					Server/RequestProcessor/ClientHandler.cpp \
					Server/ServerCreator.cpp \
					Server/Locations.cpp \
					Server/Server.cpp \
					Server/Selector.cpp \
					Server/RequestProcessor/RequestHandler/GetRequestHandler.cpp \
					Server/RequestProcessor/RequestHandler/PostRequestHandler.cpp \
					Server/RequestProcessor/RequestHandler/CommonResponseSender.cpp \
					Server/RequestProcessor/RequestHandler/CGI.cpp \
					Server/RequestProcessor/RequestHandler/ErrorSender.cpp \

SRCS		= $(addprefix $(SRC_DIR), $(SRCS_NAMES))

NUM_THREADS	= $(shell nproc)

ifeq ($(MODE), Debug)
	CXXFLAGS += -D_DEBUG -g -fsanitize=address
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
	@cd test_files/cgi-bin && $(MAKE) && cd ../..

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
	@rm -f PVS-Studio.log
	@rm -f report.tasks
	@rm -f strace_out
	@rm -f test_files/cgi-bin/CgiTest

re: fclean all

-include $(DEPS)
.PHONY:	all, clean, fclean, re
