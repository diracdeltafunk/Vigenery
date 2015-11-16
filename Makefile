CXX = clang++
RM = rm -f
CPPFLAGS = -g -Wall -O2 -std=c++11 -pthread
SRCS = util.cpp affine.cpp vigenere.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = vigenery

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(OBJS)

util.o: util.cpp util.h
	$(CXX) $(CPPFLAGS) -c util.cpp

affine.o: affine.cpp affine.h util.h
	$(CXX) $(CPPFLAGS) -c affine.cpp

vigenere.o: vigenere.cpp vigenere.h util.h
	$(CXX) $(CPPFLAGS) -c vigenere.cpp

main.o: main.cpp affine.h vigenere.h
	$(CXX) $(CPPFLAGS) -c main.cpp

clean:
	$(RM) $(OBJS) $(TARGET) *~
