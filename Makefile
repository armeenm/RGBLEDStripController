CC = g++
TARGET = sendcolorsv2
SRCS = sendcolorsv2.cpp
LIBS = -lpthread -lboost_system
CXXFLAGS = -Wall -Wpedantic -std=c++17 $(LIBS)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SRCS)
