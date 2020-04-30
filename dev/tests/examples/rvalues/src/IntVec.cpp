// TODO add Copyright

#include "IntVec.h"

#include <cstring>

namespace jmf
{
namespace tests
{
namespace examples
{

IntVec::IntVec(std::shared_ptr<int> counter, size_t num)
    :
    counter_(counter),
    size_(num),
    data_(new int[size_])
{
  Log("Constructor: IntVec(std::shared_ptr<int> counter, size_t num)");
  if (counter_)
  {
    ++(*counter_);
  }
}

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
  if (counter_)
  {
    ++(*counter_);
  }

  if (data_)
  {
    delete[] data_;
    data_ = 0;
  }
}

IntVec::IntVec(const IntVec &other)
    :
    counter_(other.counter_),
    size_(other.size_),
    data_(new int[size_])
{
  Log("copy constructor");

  std::memcpy(data_, other.data_, sizeof(*data_) * size_);

  if (counter_)
  {
    ++(*counter_);
  }
}

IntVec& IntVec::operator=(const IntVec &other)
{
  Log("copy assignment operator");
  IntVec tmp(other);
  std::swap(size_, tmp.size_);
  std::swap(data_, tmp.data_);
  return *this;
}

IntVec& IntVec::operator=(IntVec &&other)
{
  Log("move assignment operator");
  std::swap(size_, other.size_);
  std::swap(data_, other.data_);
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

