// TODO add Copyright
#include <cstring>
#include <memory>

#include <gtest/gtest.h>

#include "mocks/IntVecMock.h"
#include "IntVec.h"

using ::testing::_;

namespace jmf
{
namespace tests
{
namespace examples
{

TEST(IntVec_Test, Size)
{
  // this test case use private member to exercise the use of:
  // FRIEND_TEST(IntVec_Test, Size);
  const size_t kVecSize = 20;

  IntVec v1(kVecSize);
  IntVec v2;

  EXPECT_EQ(v1.size_, kVecSize);
  EXPECT_EQ(v2.size_, 0);

  v2 = v1;

  EXPECT_EQ(v1.size_, v2.size_);
  EXPECT_EQ(v1.size_, kVecSize);
}

TEST(IntVec_Test, Fill)
{
  const size_t kVecSize = 20;

  IntVec v1(kVecSize);

  EXPECT_EQ(v1.size(), kVecSize);

  v1.Fill(0xdeadbeef);

  for (size_t i = 0; i < kVecSize; ++i)
  {
    EXPECT_EQ(v1.at(i), 0xdeadbeef);
  }
}

TEST(IntVec_Test, FillLog)
{
  const size_t kVecSize = 20;

  IntVecMock v1(kVecSize);

  EXPECT_CALL(v1, Log(_)).Times(1);
  EXPECT_EQ(v1.size(), kVecSize);

  v1.Fill(0xdeadbeef);

  for (size_t i = 0; i < kVecSize; ++i)
  {
    EXPECT_EQ(v1.at(i), 0xdeadbeef);
  }
}

TEST(IntVec_Test, AssigmentOperator)
{
  // this test case use private member to exercise the use of:
  // FRIEND_TEST(IntVec_Test, AssigmentOperator);
  const size_t kVecSize = 20;

  IntVec v1(kVecSize);
  IntVec v2;

  v1.Fill(0xdeadbeef);

  v2 = v1;

  EXPECT_EQ(v1.size_, v2.size_);
  EXPECT_EQ(v1.size_, kVecSize);

  EXPECT_EQ(std::memcmp(v1.data_, v2.data_, kVecSize), 0);
  // this for is redundant once the above EXPECT_EQ tests the same thing
  // it's here only to demonstrate another to test it
  for (size_t i = 0; i < v1.size_; ++i)
  {
    EXPECT_EQ(v1.data_[i], 0xdeadbeef);
    EXPECT_EQ(v2.data_[i], 0xdeadbeef);
  }
}

TEST(IntVec_Test, MoveAssigmentOperator)
{
  const size_t kVecSize = 20;

  {
    IntVec v1(kVecSize);
    IntVec v2;

    v1.Fill(0xdeadbeef);

    v2 = std::move(v1);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.get(), nullptr);
    EXPECT_EQ(v2.size(), kVecSize);
    EXPECT_NE(v2.get(), nullptr);

    for (size_t i = 0; i < kVecSize; ++i)
    {
      EXPECT_EQ(v2.at(i), 0xdeadbeef);
    }
  }
}

}
}
}
