// Copyright 2019 Stefan Silviu

#pragma once

#include <cstdint>
#include <cstring>
#include <string>

namespace microloop
{

class Buffer
{
  std::size_t sz;
  void *buf;

public:
  Buffer() noexcept : sz{0}, buf{nullptr}
  {}

  explicit Buffer(std::size_t sz) noexcept : sz(sz), buf(malloc(sz))
  {}

  Buffer(const char *src, std::size_t sz) : Buffer(sz)
  {
    std::memcpy(buf, src, sz);
  }

  Buffer(const char *src) : Buffer(strlen(src) + 1)
  {
    std::memcpy(buf, src, sz);
  }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
  Buffer(const std::string &src) : Buffer(src.c_str(), src.length())
  {}
#pragma clang diagnostic pop

  ~Buffer()
  {
    free(buf);
  }

  Buffer(const Buffer &other) noexcept : Buffer(other.sz)
  {
    std::memcpy(buf, other.buf, other.sz);
  }

  Buffer &operator=(Buffer other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  Buffer(Buffer &&) noexcept = default;
  Buffer &operator=(Buffer &&) noexcept = default;

  void *data() const noexcept
  {
    return buf;
  }

  std::size_t size() const noexcept
  {
    return sz;
  }

  bool empty() const noexcept
  {
    return sz == 0;
  }

  void resize(std::size_t new_size) noexcept
  {
    buf = realloc(buf, new_size);
    sz = new_size;
  }

  friend void swap(Buffer &lhs, Buffer &rhs) noexcept
  {
    using std::swap;
    swap(lhs.sz, rhs.sz);
    swap(lhs.buf, rhs.buf);
  }
};
}  // namespace microloop
