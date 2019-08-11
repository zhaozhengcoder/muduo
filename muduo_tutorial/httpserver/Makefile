include ../inc.mk

MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = ./

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

APP_CFILE = $(wildcard $(SRC)/*.cc)
APP_EXE = http_server

all: $(APP_EXE)

clean:
	rm -f *.core $(APP_EXE)

$(APP_EXE): $(APP_CFILE)
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean