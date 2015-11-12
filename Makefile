CXX = clang++
RM = rm -f
CPPFLAGS = -g -Wall -O2 -std=c++11
SRCS = util.cpp affine.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = vigenery

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(OBJS)

util.o: util.cpp util.h
	$(CXX) $(CPPFLAGS) -c util.cpp

affine.o: affine.cpp affine.h util.h util.o
	$(CXX) $(CPPFLAGS) -c affine.cpp

main.o: main.cpp affine.h affine.o
	$(CXX) $(CPPFLAGS) -c main.cpp

clean:
	$(RM) $(OBJS) $(TARGET) *~
