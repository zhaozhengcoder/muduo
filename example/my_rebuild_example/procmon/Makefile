MUDUO_DIRECTORY ?= $(HOME)/code_test/muduo/build/release-install
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib

CXXFLAGS = -I$(MUDUO_INCLUDE) -I/usr/local/libgd/include
LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_http -lmuduo_net_cpp11 -lmuduo_base_cpp11  -lmuduo_inspect -lpthread -lrt 

# 刚才一直报错，报错的原因是连接静态库的顺序出错了 把lmuduo_http 放到最前面，就可以了 
# 原因是lib的相互依赖

cc = g++
src = plot.cc procmon.cc
flag = -std=c++11 -Wall -Wextra -g 

procmon:$(src)
	$(cc) $(flag) $(CXXFLAGS) -o procmon $(src) $(LDFLAGS) -L /usr/local/libgd/lib/ -lgd


dummyload:dummyload.cc
	$(cc) $(flag) $(CXXFLAGS) -o dummyload dummyload.cc $(LDFLAGS)

plot_test:plot_test.cc plot.cc
	$(cc) $(flag) $(CXXFLAGS) -o plot_test plot_test.cc plot.cc $(LDFLAGS) -L /usr/local/libgd/lib/ -lgd

