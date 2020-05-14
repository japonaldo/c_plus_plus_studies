#include "CtorCounter.h"

#include <iostream>

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

/*
 * This code is an adaption from Effective Modern C++ of Scott Meyers, item 25.
 */

namespace jmf
{
namespace tests
{
namespace examples
{
namespace item25
{

class Rvo_Test : public ::testing::Test
{
 public:
  void SetUp()
  {
    CtorCounter<int>::copyctor_counter_ = 0;
    CtorCounter<int>::movector_counter_ = 0;
    CtorCounter<int>::dtor_counter_ = 0;
  }

  void TearDown()
  {
  }
};

/*
 * Return value optimization (RVO) can avoid the need to copy the local variable by
 * constructing it in the memory alloted for the function’s return value.
 *
 * this particular blessing says that compilers may elide the copying (or moving)
 * of a local object2 in a function that returns by value if:
 * (1) the type of the local object is the same as that returned by the function and
 * (2) the local object is what’s being returned
 */

template<typename T>
CtorCounter<T> MakeCtorCounterNRVO(bool arg)  // NRVO - Named Return Value Optimization
{
  CtorCounter<T> counter;

  if (arg)
  {
    // there can also be lots of code here
    // it'll not hinder the compiler to apply RVO, though

    return counter;
  }

  return counter;
}

template<typename T>
CtorCounter<T> MakeCtorCounterRVO1()
{
  return CtorCounter<T>();
}

template<typename T>
CtorCounter<T> MakeCtorCounterRVO2()
{
  return MakeCtorCounterNRVO<T>(true);
}

/*
 * Why NRVO/RVO is not applied in this case?
 *
 * Condition (2) stipulates that the RVO may be performed
 * only if what’s being returned is a local object.
 *
 * What’s being returned here isn’t the local object "counter",
 * it’s a reference to w—the result of std::move(counter).
 * Returning a reference to a local object doesn’t satisfy the conditions
 * required for the RVO, so compilers must move "counter"
 * into the function’s return value location.
 * Developers trying to help their compilers optimize by applying std::move
 * to a local variable that’s being returned are actually limiting
 * the optimization options available to their compilers!
 * But the RVO is an optimization. Compilers aren’t required to elide copy
 * and move operations, even when they’re permitted to.
 * That is, you might still think it’s reasonable to apply std::move to a local
 * object you’re returning, simply because you’d rest easy knowing
 * you’d never pay for a copy.
 * In that case, applying std::move to a local object would still be a bad idea.
 * The part of the Standard blessing the RVO goes on to say that if the conditions
 * for the RVO are met, but compilers choose not to perform copy elision,
 * the object being returned must be treated as an rvalue.
 * In effect, the Standard requires that when the RVO is permitted,
 * either copy elision takes place or std::move is implicitly applied to local
 * objects being returned.
 * So in the “copying” version (MakeCtorCounterNRVO, MakeCtorCounterRVO),
 * compilers must either elide the copying of "counter" or
 * they must treat the function as if it were written like this version (with std::move).
 *
 * This means that if you use std::move on a local object being returned from a
 * function that’s returning by value, you can’t help your compilers
 * (they have to treat the local object as an rvalue if they don’t perform
 * copy elision), but you can certainly hinder them (by precluding the RVO)
 */
template<typename T>
CtorCounter<T> MakeCtorCounterMove()
{
  CtorCounter<T> counter;

  return std::move(counter);
}

/*
 * The situation is similar for by-value function parameters.
 * They’re not eligible for copy elision with respect to their function’s return value,
 * but compilers must treat them as rvalues if they’re returned.
 */
template<typename T>
CtorCounter<T> MakeCtorCounterNRVO(CtorCounter<T> counter, T val)  // NRVO - Named Return Value Optimization
{
  counter.data_ = val;
  /*
   * As a result, compilers must treat it as if it had been written this way
   * return std::move(counter);
   */
  return counter;
}

TEST_F(Rvo_Test, MakeCtorCounterNRVOWithFalse)
{
  {
    CtorCounter<int> counter = MakeCtorCounterNRVO<int>(false);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
}

TEST_F(Rvo_Test, MakeCtorCounterNRVOWithTrue)
{
  {
    CtorCounter<int> counter = MakeCtorCounterNRVO<int>(true);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
}

TEST_F(Rvo_Test, MakeCtorCounterRVO1)
{
  {
    CtorCounter<int> counter = MakeCtorCounterRVO1<int>();

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
}

TEST_F(Rvo_Test, MakeCtorCounterRVO2)
{
  {
    CtorCounter<int> counter = MakeCtorCounterRVO2<int>();

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 0);
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 0);
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
}

TEST_F(Rvo_Test, MakeCtorCounterMove)
{
  {
    CtorCounter<int> counter = MakeCtorCounterMove<int>();

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);  // copy elision performed
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 1);  // move constructor is invoke, though
                                                        // to tmp object
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);  // destructor is invoke as well,
                                                    // to detroy the tmp object
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

TEST_F(Rvo_Test, MakeCtorCounterNRVOByValueParam)
{
  {
    CtorCounter<int> counter = MakeCtorCounterNRVO<int>(CtorCounter<int>(10),
                                                        20);

    EXPECT_EQ(CtorCounter<int>::copyctor_counter_, 0);  // copy elision performed
    EXPECT_EQ(CtorCounter<int>::movector_counter_, 1);  // move constructor is invoke
    EXPECT_EQ(CtorCounter<int>::dtor_counter_, 1);
    EXPECT_EQ(counter.data_, 20);
  }
  EXPECT_EQ(CtorCounter<int>::dtor_counter_, 2);
}

}
}
}
}

