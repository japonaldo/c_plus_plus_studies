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

/*
 * This code is an adaption from Effective Modern C++ of Scott Meyers
 */
class ForceCopyCtor
{
 public:
  explicit ForceCopyCtor(const CtorCounter<int> ctor)
      :
      /*
       Adapted from: item 23

       ctor is NOT MOVED into value_, it’s COPIED.
       ctor is cast to an rvalue by std::move,
       ctor is declared to be a const CtorCounter<int>, though
       before the cast,
       - ctor is an lvalue const CtorCounter<int>,
       - and the result of the cast is an rvalue const CtorCounter<int>,
       throughout it all, the constness remains.

       ***
       The code below doesn't do what it seems to.
       The result of std::move(ctor) is an rvalue of type const CtorCounter<int>
       It can’t be passed to CtorCounter<int>'s move constructor,
       because the move constructor takes an rvalue reference to a
       NON-CONST CtorCounter<int>.
       It can, however, be passed to the copy constructor,
       because an lvalue-reference-to-const is permitted to bind to a const rvalue.
       */
      value_(std::move(ctor))
  {
    /*
     Two lessons from the above case:
     1 - Move requests on const objects are silently transformed into copy operations
     2 - std::move not only doesn’t actually move anything,
     it doesn’t even guarantee that the object it’s casting will be eligible
     to be moved.
     The only thing you know for sure about the result of applying
     std::move to an object is that it’s an rvalue
     */
  }

  ~ForceCopyCtor()
  {
  }

  ForceCopyCtor() = delete;
  ForceCopyCtor(ForceCopyCtor&&) = delete;
  ForceCopyCtor& operator=(ForceCopyCtor&) = delete;
  ForceCopyCtor& operator=(ForceCopyCtor&&) = delete;

  CtorCounter<int> value_;
};

TEST(CtorCounter_Test, ForceCopyCtor)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    // this shall not invoke CtorCounter's copy constructor
    ForceCopyCtor force_copyctor(CtorCounter<int>(10));

    // as explained above,
    // value_(std::move(ctor)) DOESN'T invoke the Move Constructor,
    // it DOES invoke the Copy Constructor
    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    // the "inner/temporarily" object,
    // created in value_(std::move(ctor)) gets destroyed
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
    EXPECT_EQ(force_copyctor.value_.data_, 10);
  }
  // now, the local (force_copyctor.value_) instance gets destroyed
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

TEST(CtorCounter_Test, Force2CopyCtor)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> ctor(10);

    // this shall invoke CtorCounter's Copy Constructor
    ForceCopyCtor force_copyctor(ctor);

    // as expected, the line above invoked the copy constructor
    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 2);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
    EXPECT_EQ(force_copyctor.value_.data_, 10);
  }

  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 3);
}

TEST(CtorCounter_Test, TryMoveLocalCtorCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    CtorCounter<int> ctor(10);

    // this line shall invoke CtorCounter's Move Constructor
    ForceCopyCtor force_copyctor(std::move(ctor));

    // as expected, the line above invoked the move constructor
    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 1);
    // the "inner/temporarily" object,
    // created in value_(std::move(ctor)) is the only one destroyed
    // inside this block statement
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
    EXPECT_EQ(force_copyctor.value_.data_, 10);
  }

  // now, ctor and force_copyctor.value_ gets destroyed
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 3);
}

TEST(CtorCounter_Test, TryMoveCtorCounter)
{
  CtorCounter<int>::copyctor_counter_ = 0;
  CtorCounter<int>::movector_counter_ = 0;
  CtorCounter<int>::dtor_counter_ = 0;

  {
    // this line, as in the previous test,
    // shall also invoke CtorCounter's Move Constructor
    ForceCopyCtor force_copyctor(std::move(CtorCounter<int>(10)));

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 1);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 1);
    // the "intermediate" object created in
    // ForceCopyCtor force_copyctor(std::move(CtorCounter<int>(10)));
    // is destroyed inside this block statement,
    // making the difference to the previous test
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
    EXPECT_EQ(force_copyctor.value_.data_, 10);
  }

  // now, force_copyctor.value_ gets destroyed
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 3);
}

}
}
}
