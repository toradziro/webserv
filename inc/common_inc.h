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

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#include <Errors.hpp>