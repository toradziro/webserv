#pragma once

#include <string>

#include <vector>
#include <map>
#include <unordered_map>
#include <utility>

#include <iostream>
#include <fstream>
#include <exception>
#include <cassert>
#include <csignal>
#include <memory>
#include <utility>
#include <filesystem>
#include <charconv>
#include <algorithm>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#include <Errors.hpp>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
