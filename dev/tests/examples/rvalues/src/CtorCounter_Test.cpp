#include "CtorCounter.h"

#include <gtest/gtest.h>

namespace jmf
{
namespace tests
{
namespace examples
{

TEST(CtorCounter_Test, CopyCtorCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> obj_1(10);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    CtorCounter<int> obj_2(obj_1);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);
  }

  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

TEST(CtorCounter_Test, MoveCtorCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> obj_1(10);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    CtorCounter<int> obj_2(std::move(obj_1));

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    EXPECT_EQ(obj_2.data_, 10);
  }

  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

TEST(CtorCounter_Test, CopyAssignmentCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> obj_1(10);
    CtorCounter<int> obj_2;

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    obj_2 = obj_1;

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);

    EXPECT_EQ(obj_2.data_, 10);
  }

  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 3);
}

TEST(CtorCounter_Test, MoveAssignmentCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> obj_1(10);
    CtorCounter<int> obj_2;

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    obj_2 = std::move(obj_1);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);

    EXPECT_EQ(obj_2.data_, 10);
  }

  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

}
}
}
