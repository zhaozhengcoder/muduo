MUDUO_DIRECTORY ?= $(HOME)/code_test/muduo/build/release-install
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = ./

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY)  -lmuduo_net  -lmuduo_base -lpthread -lrt

# lmuduo_inspect 
all: hub sub
clean:
	rm -f hub core

hub: $(SRC)/hub.cc codec.cc
	g++ $(CXXFLAGS) -o $@ $^  $(LDFLAGS)

sub: $(SRC)/sub.cc codec.cc pubsub.cc
	g++ $(CXXFLAGS) -o $@ $^  $(LDFLAGS)

# pub: $(SRC)/pub.cc codec.cc pubsub.cc
# 	g++ $(CXXFLAGS) -o $@ $^  $(LDFLAGS)

.PHONY: all clean