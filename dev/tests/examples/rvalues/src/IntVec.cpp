// TODO add Copyright

#include "IntVec.h"

#include <cstring>

namespace jmf
{
namespace tests
{
namespace examples
{

IntVec::IntVec(size_t num)
    :
    size_(num),
    data_(new int[size_])
{
  Log("Constructor: IntVec(size_t num)");
}

IntVec::~IntVec()
{
  Log("Destructor");
  if (data_)
  {
    delete[] data_;
    data_ = nullptr;
  }
}

IntVec::IntVec(const IntVec &other)
    :
    size_(other.size_),
    data_(new int[size_])
{
  Log("copy constructor");

  std::memcpy(data_, other.data_, sizeof(*data_) * size_);
}

IntVec& IntVec::operator=(const IntVec &other)
{
  if (this != &other)
  {
    Log("copy assignment operator");
    IntVec tmp(other);
    std::swap(size_, tmp.size_);
    std::swap(data_, tmp.data_);
  }
  return *this;
}

IntVec& IntVec::operator=(IntVec &&other)
{
  if (this != &other)
  {
    Log("move assignment operator");
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
    other.size_ = 0;
    other.data_ = nullptr;
  }
  return *this;
}

void IntVec::Fill(int val)
{
  Log("Fill(int val)");
  for (size_t i = 0; i < size_; ++i)
  {
    data_[i] = val;
  }
}

void IntVec::Log(const char *msg)
{
  std::cout << "[" << this << "] " << msg << "\n";
}

}
}
}

