// SPDX-License-Identifier: MIT
#pragma once
#include <cstddef>
#include <cstdint>
class Print {
public:
  virtual ~Print() = default;
  virtual size_t write(uint8_t) = 0;
  size_t write(const uint8_t *data, size_t n) {
    size_t written = 0;
    while (written < n && write(data[written])) ++written;
    return written;
  }
};
class Stream : public Print {
public:
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int peek() = 0;
  virtual void flush() = 0;
};
