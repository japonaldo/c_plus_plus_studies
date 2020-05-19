#include <gtest/gtest.h>

/*
 * This code stem from:
 * 1: https://eli.thegreenplace.net/2014/variadic-templates-in-c/
 * 2: https://www.modernescpp.com/index.php/c-insights-variadic-templates
 */

namespace jmf
{
namespace tests
{
namespace examples
{

template<int...>
struct AddInt
{
};

template<>
struct AddInt<>
{
  static constexpr int value = 0;
};

template<int i, int ... tail>
struct AddInt<i, tail...>
{
  static constexpr int value = i + AddInt<tail...>::value;
};

template<typename T, size_t n>
constexpr size_t ArrSize(T (&arr)[n])
{
  return sizeof(arr);
}
;

TEST(VariadicTemplate_Test, AddIntConstexpr)
{
  // do not compile, see below the error messages from gcc
  // error: conversion from ‘double’ to ‘int’ not considered for non-type template argument
  // error: could not convert template argument ‘1.0e+0’ from ‘double’ to ‘int’
  // EXPECT_EQ((AddInt<1.0>::value), 1);

  // do not compile, see below the error messages from gcc
  // error: conversion from ‘float’ to ‘int’ not considered for non-type template argument
  // error: could not convert template argument ‘1.0e+0f’ from ‘float’ to ‘int’
  // EXPECT_EQ((AddInt<1.0f>::value), 1);

  static_assert((AddInt<>::value) == 0);
  static_assert((AddInt<1>::value) == 1);
  static_assert((AddInt<1, 1>::value) == 2);
  static_assert((AddInt<2, 2, 4>::value) == 8);
  static_assert((AddInt<2, 2, 4, 8>::value) == 16);
  static_assert((AddInt<2, 2, 4, 8, 16>::value) == 32);
  static_assert((AddInt<1, 1, 1, 1, 1, 1>::value) == 6);

  EXPECT_EQ((AddInt<>::value), 0);
  EXPECT_EQ((AddInt<1>::value), 1);
  EXPECT_EQ((AddInt<1, 1>::value), 2);
  EXPECT_EQ((AddInt<2, 2, 4>::value), 8);
  EXPECT_EQ((AddInt<2, 2, 4, 8>::value), 16);
  EXPECT_EQ((AddInt<2, 2, 4, 8, 16>::value), 32);
  EXPECT_EQ((AddInt<1, 1, 1, 1, 1, 1>::value), 6);
}

TEST(VariadicTemplate_Test, ArrSize)
{
  {
    char arr[10] =
      { 0 };

    EXPECT_EQ(ArrSize<char>(arr), 10 * sizeof(char));
    static_assert(ArrSize<char>(arr) == 10 * sizeof(char));
  }
  {
    int arr[10] =
      { 0 };

    EXPECT_EQ(ArrSize<int>(arr), 10 * sizeof(int));
    static_assert(ArrSize<int>(arr) == 10 * sizeof(int));
  }
  {
    short arr[10] =
      { 0 };

    EXPECT_EQ(ArrSize<short>(arr), 10 * sizeof(short));
    static_assert(ArrSize<short>(arr) == 10 * sizeof(short));
  }
}

}
}
}
