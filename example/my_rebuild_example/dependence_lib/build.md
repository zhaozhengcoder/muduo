
```
1.安装zlib
tar zxvf zlib*.gz
cd zlib*/
./configure --prefix=/usr/local/zlib
make && make install
        
2.安装libpng
tar xzf libpng*.gz
cd  libpng*/
./configure --prefix=/usr/local/libpng
make && make install
        
3.安装freetype
tar xzf freetype*.tar.gz
cd  freetype*/
./configure --prefix=/usr/local/freetype
make && make install && make clean
        
4.安装Jpeg
tar xzf jpeg*.gz
cd  jpeg*/
./configure --prefix=/usr/local/libjpeg
sed -i 's/CFLAGS\= -O2  -I\$(srcdir)/CFLAGS\= -O3 -I\$(srcdir) -fPIC/g' Makefile
mkdir -p /usr/local/libjpeg/include
mkdir -p /usr/local/libjpeg/lib
mkdir -p /usr/local/libjpeg/bin
mkdir -p /usr/local/libjpeg/man/man1
make && make install-lib && make install
        
5.安装GD
tar zxf libgd*.gz
cd libgd*/
./configure --prefix=/usr/local/libgd --with-png=/usr/local/libpng/ --with-jpeg=/usr/local/libjpeg/ --with-freetype=/usr/local/freetype/
make && make install
```