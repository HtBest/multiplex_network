
# Compiler
CC = clang++

# Compiler flags
CFLAGS = -w -g -O3 -std=c++2b -fsanitize=address
# CFLAGS = -w -g -O3 -std=c++2b -march=native -fno-exceptions -flto -funroll-loops #-fsanitize=address

#  database.cpp

EXECS = main

# Header files
HDRS = graph.hpp

# Source files
SRCS = graph.cpp

# Object files
# Default target
all: $(SUBDIRS) $(EXECS)

$(SUBDIRS):
	$(MAKE) -C $@

main: main.cpp $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) main.cpp
	