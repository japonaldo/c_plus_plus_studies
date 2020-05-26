#include <stddef.h>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <utility>

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

template<typename T>
T AddAny(T t)
{
  return t;
}

template<typename T, typename ... Args>
T AddAny(T first, Args ... args)
{
  return first + AddAny(args...);
}

TEST(VariadicTemplate_Test, AddAnyInt)
{
  EXPECT_EQ(AddAny(1, 1), 2);
  EXPECT_EQ(AddAny(1, 1, 2, 4), 8);
  EXPECT_EQ(AddAny(1, 1, 2, 4, 3), 11);
  EXPECT_EQ(AddAny(1, 1, 2, 4.5, 3), 11);
  EXPECT_EQ(AddAny(1.5, 1, 2, 4.5, 3.5), 12.5);
}

template<typename T>
constexpr bool ComparePair(T a)
{
  return false;
}

template<typename T>
constexpr bool ComparePair(T a, T b)
{
  return a == b;
}

template<typename T, typename ... Args>
constexpr bool ComparePair(T a, T b, Args ... args)
{
  return (a == b) && ComparePair(args...);
}

TEST(VariadicTemplate_Test, ComparePairIntFalses)
{
  static_assert(ComparePair(0) == false);
  static_assert(ComparePair(0, 1) == false);
  static_assert(ComparePair(1, 1, 2) == false);
  static_assert(ComparePair(1, 1, 2, 5) == false);
  static_assert(ComparePair(1, 1, 2, 2, 6, 3) == false);

  EXPECT_EQ(ComparePair(0), false);
  EXPECT_EQ(ComparePair(0, 1), false);
  EXPECT_EQ(ComparePair(1, 1, 2), false);
  EXPECT_EQ(ComparePair(1, 1, 2, 5), false);
  EXPECT_EQ(ComparePair(1, 1, 2, 2, 6, 3), false);
}

TEST(VariadicTemplate_Test, ComparePairExpectIntTrues)
{
  static_assert(ComparePair(1, 1) == true);
  static_assert(ComparePair(1, 1, 2, 2) == true);
  static_assert(ComparePair(1, 1, 2, 2, 4, 4) == true);

  EXPECT_EQ(ComparePair(1, 1), true);
  EXPECT_EQ(ComparePair(1, 1, 2, 2), true);
  EXPECT_EQ(ComparePair(1, 1, 2, 2, 4, 4), true);
}

TEST(VariadicTemplate_Test, ComparePairDoubleFalses)
{
  static_assert(ComparePair(0.0) == false);
  static_assert(ComparePair(0.75, 1.75) == false);
  static_assert(ComparePair(1.75, 1.75, 2.99) == false);
  static_assert(ComparePair(1.75, 1.75, 2.99, 5.99) == false);
  static_assert(ComparePair(1.75, 1.75, 2.99, 2.99, 6.123, 3.123) == false);

  EXPECT_EQ(ComparePair(0.0), false);
  EXPECT_EQ(ComparePair(0.75, 1.75), false);
  EXPECT_EQ(ComparePair(1.75, 1.75, 2.99), false);
  EXPECT_EQ(ComparePair(1.75, 1.75, 2.99, 5.99), false);
  EXPECT_EQ(ComparePair(1.75, 1.75, 2.99, 2.99, 6.123, 3.123), false);
}

TEST(VariadicTemplate_Test, ComparePairExpectDoubleTrues)
{
  static_assert(ComparePair(1.55, 1.55) == true);
  static_assert(ComparePair(1.55, 1.55, 2.99, 2.99) == true);
  static_assert(ComparePair(1.55, 1.55, 2.99, 2.99, 4.123, 4.123) == true);

  EXPECT_EQ(ComparePair(1.55, 1.55), true);
  EXPECT_EQ(ComparePair(1.55, 1.55, 2.99, 2.99), true);
  EXPECT_EQ(ComparePair(1.55, 1.55, 2.99, 2.99, 4.123, 4.123), true);
}

TEST(VariadicTemplate_Test, ComparePairExpectIntVsDouble)
{
  // the following doesn't compile, gcc gives the following error message:
  // error: no matching function for call to ‘ComparePair(int, double)’
  // EXPECT_EQ(ComparePair(1, 1.6), true);

  // However, the following compiles and the EXPECT_EQ passes
  // because 1.6 is being implicitly converted to int
  static_assert(ComparePair<int>(1, 1.6) == true);

  EXPECT_EQ(ComparePair<int>(1, 1.6), true);
}

template<class ... Ts>
struct Tuple {};

template<class T, class ... Ts>
struct Tuple<T, Ts...>  : Tuple<Ts...>
{
  Tuple(T t, Ts ... ts) : Tuple<Ts...>(ts...),
                          value(t)
  {
  }

  T value;
};

template<size_t, class>
struct ElementTypeHolder {};

template<class T, class... Ts>
struct ElementTypeHolder<0, Tuple<T, Ts...>>
{
  typedef T type;
};

template<size_t K, class T, class... Ts>
struct ElementTypeHolder<K, Tuple<T, Ts...>>
{
  typedef typename ElementTypeHolder<K - 1, Tuple<Ts...>>::type type;
};

template<size_t K, class... Ts>
typename std::enable_if<K == 0, 
                        typename ElementTypeHolder<0, Tuple<Ts...>>::type&>::type
get(Tuple<Ts...>& t)
{
  return t.value;
}

template<size_t K, class T, class... Ts>
using ElementTypeHolder_t = typename ElementTypeHolder<K, Tuple<T, Ts...>>::type;

template<size_t K, class T, class... Ts>
using ElementTypeHolder_tref = ElementTypeHolder_t<K, T, Ts...>&;

template<size_t K, class T, class... Ts>
std::enable_if_t<K != 0, ElementTypeHolder_tref<K, T, Ts...>>
get(Tuple<T, Ts...>& t)
{
  Tuple<Ts...>& base = t;
  return get<K - 1, Ts...>(base);
}

TEST(VariadicTemplate_Test, Tuple)
{
  Tuple<int, double, const char*, bool, float> tuple(10,
                                                     5.5,
                                                     "My Tuple's String",
                                                     true,
                                                     1.5f);

  EXPECT_EQ(get<0>(tuple), 10);
  EXPECT_EQ(get<1>(tuple), 5.5);
  EXPECT_EQ(get<2>(tuple), "My Tuple's String");
  EXPECT_EQ(get<3>(tuple), true);
  EXPECT_EQ(get<4>(tuple), 1.5f);
}

template<template<typename, typename...> 
         class ContainerType, typename ValueType, typename... Args>
void PrintMyContainer(const ContainerType<ValueType, Args...>& c)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  using std::cout;
  for (const auto& v : c)
  {
    cout << v << ' ';
  }
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p)
{
  os << "[" << p.first << ", " << p.second << "]";
  return os;
}

TEST(VariadicTemplate_Test, PrintMyVector)
{
  std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  PrintMyContainer(v);

  std::cout << std::endl;
}

TEST(VariadicTemplate_Test, PrintMyList)
{
  std::list<int> list{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

  PrintMyContainer(list);

  std::cout << std::endl;
}

TEST(VariadicTemplate_Test, PrintMyMap)
{
  std::map<int, std::string> map{
      {0, "Null"},
      {1, "Eins"},
      {2, "Zwei"},
      {3, "Drei"},
      {4, "Vier"},
      {5, "Fünf"},
      {6, "Sechs"},
      {7, "Sieben"},
      {8, "Acht"},
      {9, "Neun"},
      {10, "Zehn"},
  };

  PrintMyContainer(map);

  std::cout << std::endl;
}

}
}
}
