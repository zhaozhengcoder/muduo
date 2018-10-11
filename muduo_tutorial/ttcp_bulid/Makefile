MUDUO_DIRECTORY ?= $(HOME)/code_test/muduo/build/release-install
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = ../ttcp

MUDDUO_EXPAMLE_LIB = $(HOME)/code_test/muduo/muduo-master


CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -std=c++11 \
	   -I$(MUDUO_INCLUDE) \
	   -I$(MUDDUO_EXPAMLE_LIB)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt

all: ttcp
clean:
	rm -f ttcp core

ttcp: $(SRC)/ttcp_blocking.cc $(SRC)/ttcp_main.cc  $(SRC)/common.cc 
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean