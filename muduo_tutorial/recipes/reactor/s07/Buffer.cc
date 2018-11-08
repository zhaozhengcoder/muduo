// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Buffer.h"
#include "SocketsOps.h"
#include "logging/Logging.h"

#include <errno.h>
#include <memory.h>
#include <sys/uio.h>

using namespace muduo;

// #include <sys/uio.h>
// struct iovec {
//     ptr_t iov_base; /* Starting address */
//     size_t iov_len; /* Length in bytes */
// };

// struct iovec定义了一个向量元素。通常，这个结构用作一个多元素的数组。
// 对于每一个传输的元素，指针成员iov_base指向一个缓冲区，这个缓冲区是存放的是readv所接收的数据或是writev将要发送的数据。
// 成员iov_len在各种情况下分别确定了接收的最大长度以及实际写入的长度。

// 将fd里面可读的数据，放到this->buffer_ 里面
ssize_t Buffer::readFd(int fd, int *savedErrno) {
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin() + writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;

  const ssize_t n = readv(
      fd, vec, 2); // 关于readv 系统调用，
                   // https://blog.csdn.net/opens_tym/article/details/7999151
  if (n < 0) {
    *savedErrno = errno;
  } else if (implicit_cast<size_t>(n) <= writable) {
    writerIndex_ += n;
  } else {
    writerIndex_ = buffer_.size();
    append(extrabuf,
           n - writable); // 将extrabuf里面的数据放到this->buffer_ 里面
  }
  return n;
}
