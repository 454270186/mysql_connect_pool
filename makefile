# Makefile for compiling MySQL C++ API on Ubuntu

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++14 -I/usr/include/mysql -DBIG_JOINS=1 -fno-strict-aliasing -g -I./include

# Linker flags
LDFLAGS = -L/usr/lib/mysql -lmysqlclient -lpthread

# Source files
SRCS = main.cpp src/MysqlConn.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)