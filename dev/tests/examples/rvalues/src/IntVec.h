// TODO add Copyright

#ifndef DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H
#define DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H

#include <algorithm>
#include <cstddef>
#include <iostream>

#include <gtest/gtest_prod.h>

namespace jmf
{
namespace tests
{
namespace examples
{

class IntVec
{
 public:
  explicit IntVec(size_t num = 0);

  virtual ~IntVec();

  IntVec(const IntVec &other);

  IntVec& operator=(const IntVec &other);

  IntVec& operator=(IntVec &&other);

  void Fill(int val);

  const size_t size() const
  {
    return size_;
  }

  int at(size_t index)
  {
    return data_[index];
  }

  int* get()
  {
    return data_;
  }

 protected:
  virtual void Log(const char *msg);

 private:
  size_t size_;
  int *data_;

  FRIEND_TEST(IntVec_Test, Size);
  FRIEND_TEST(IntVec_Test, AssigmentOperator);
};

}
}
}

#endif //DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H
