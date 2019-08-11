include ../inc.mk

MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = ./

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt -std=c++11

all: chat_server chat_client chat_client2 chat_server2
clean:
	rm -f chat_server core

chat_server: $(SRC)/server.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

chat_server2: $(SRC)/server2.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

chat_client: $(SRC)/client.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

chat_client2: $(SRC)/client2.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean