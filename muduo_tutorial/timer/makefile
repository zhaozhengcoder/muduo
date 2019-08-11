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

LDFLAGS = -L$(MUDUO_LIBRARY)  -lmuduo_net  -lmuduo_base -lpthread -lrt    # 这个地方需要去掉 --std=c++11

# lmuduo_inspect 
all: timer1
clean:
	rm -f timer1 core

timer1: $(SRC)/timer1.cc
	g++ $(CXXFLAGS) -o $@ $^  $(LDFLAGS)

.PHONY: all clean