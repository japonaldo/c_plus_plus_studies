// TODO add Copyright

#ifndef DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H
#define DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

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
  explicit IntVec(std::shared_ptr<int> counter, size_t num = 0);
  explicit IntVec(size_t num = 0);

  virtual ~IntVec();

  IntVec(const IntVec &other);

  IntVec& operator=(const IntVec &other);

  void Fill(int val);

  const size_t size() const
  {
    return size_;
  }

  int at(size_t index)
  {
    return data_[index];
  }

 protected:
  virtual void Log(const char *msg);

 private:
  // it could perfectly be a raw pointer (int* counter_),
  // the intention is to exercise the use of std::shared_ptr, though
  std::shared_ptr<int> counter_;
  size_t size_;
  int *data_;

  FRIEND_TEST(IntVec_Test, Size);
  FRIEND_TEST(IntVec_Test, AssigmentOperator);
};

}
}
}

#endif //DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H
