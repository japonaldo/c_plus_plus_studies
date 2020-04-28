// TODO add Copyright

#include "gtest/gtest.h"

#include "IntVec.h"

namespace jmf
{
namespace tests
{
namespace examples
{

TEST(IntVec_Test, Size)
{
  const size_t kVecSize = 20;

  IntVec v1(kVecSize);
  IntVec v2;

  EXPECT_EQ(v1.size_, kVecSize);
  EXPECT_EQ(v2.size_, 0);

  v2 = v1;

  EXPECT_EQ(v1.size_, v2.size_);
  EXPECT_EQ(v1.size_, kVecSize);
}

TEST(IntVec_Test, AssigmentOperator)
{
  const size_t kVecSize = 20;

  IntVec v1(kVecSize);
  IntVec v2;

  v1.Fill(0xdeadbeef);

  v2 = v1;

  EXPECT_EQ(v1.size_, v2.size_);
  EXPECT_EQ(v1.size_, kVecSize);

  for (size_t i = 0; i < v1.size_; ++i)
  {
    EXPECT_EQ(v1.data_[i], 0xdeadbeef);
    EXPECT_EQ(v2.data_[i], 0xdeadbeef);
  }
}

}
}
}
