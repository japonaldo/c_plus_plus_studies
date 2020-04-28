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

  ~IntVec();

  IntVec(const IntVec &other);

  IntVec& operator=(const IntVec &other);

  void Fill(int val);

 private:
  void Log(const char *msg);

  size_t size_;
  int *data_;

  FRIEND_TEST(IntVec_Test, Size);
  FRIEND_TEST(IntVec_Test, AssigmentOperator);
};

}
}
}

#endif //DEV_TESTS_EXAMPLES_RVALUES_SRC_INTVEC_H
