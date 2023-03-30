# Makefile for compiling MySQL C++ API on Ubuntu

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++14 -I/usr/include/mysql -I/usr/include/jsoncpp -DBIG_JOINS=1 -fno-strict-aliasing -g -I./include

# Linker flags
LDFLAGS = -L/usr/lib/mysql -lmysqlclient -lpthread -ljsoncpp

# Source files
SRCS = connection.cpp src/MysqlConn.cpp src/ConnPool.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = connection

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)