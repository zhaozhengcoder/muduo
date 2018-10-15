#pragma once
#include <string>

class StringArg
{
public:
  StringArg(const char* str)
    : str_(str)
  { }

  StringArg(const std::string& str)
    : str_(str.c_str())
  { }

  const char* c_str() const { return str_; }

private:
  const char* str_;
};