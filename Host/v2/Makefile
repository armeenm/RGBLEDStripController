CC = g++
TARGET = sendcolorsv2
SRCS = sendcolorsv2.cpp
LIBS = -lpthread -lboost_system
CXXFLAGS = $(LIBS) -Wall -Wpedantic -std=c++17 -g

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(SRCS)
