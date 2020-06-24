#include <utility>
#include <type_traits>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace jmf
{
namespace tests
{
namespace examples
{

template<typename T,
         typename std::enable_if<std::is_reference<T>::value 
                                 && std::is_copy_constructible<T>::value, 
                                 void>::type* = nullptr>
int f(const std::remove_reference_t<T> &x)
{
  return 0;
}

template<typename T,
         typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
T f(const T &x) // T is integral in this case, so we "can" return T instead of int
{
  return 1;
}

template<typename T,
         std::enable_if_t<std::is_floating_point<T>::value>* = nullptr>
int f(const T &x)
{
  return 2;
}

TEST(EnableIf_Test, EnableIfIntegral)
{
  int i = 123;
  
  EXPECT_EQ(f<int&>(i), 0);
  EXPECT_EQ(f<int>(321), 1);
  EXPECT_EQ(f<int>(i), 1);
}

TEST(EnableIf_Test, EnableIfFloatingPoint)
{
  double d = 123.321;

  EXPECT_EQ(f<double&>(d), 0);
  EXPECT_EQ(f<double>(321.123), 2);
  EXPECT_EQ(f<double>(d), 2);
}

template<typename T, typename _ = void>
struct is_stl_like_container : std::false_type {};

template<typename... T>
struct is_stl_like_container_helper {};

template<template<typename, typename...>
         class ContainerType, typename ValueType, typename... Args>
struct is_stl_like_container<
  ContainerType<ValueType, Args...>,
  std::conditional_t<
    false,
    is_stl_like_container_helper<
      typename ContainerType<ValueType, Args...>::value_type,
      typename ContainerType<ValueType, Args...>::size_type,
      typename ContainerType<ValueType, Args...>::allocator_type,
      typename ContainerType<ValueType, Args...>::iterator,
      typename ContainerType<ValueType, Args...>::const_iterator,
      decltype(std::declval<ContainerType<ValueType, Args...>>().size()), // https://en.cppreference.com/w/cpp/utility/declval
      decltype(std::declval<ContainerType<ValueType, Args...>>().begin()),
      decltype(std::declval<ContainerType<ValueType, Args...>>().end()),
      decltype(std::declval<ContainerType<ValueType, Args...>>().cbegin()),
      decltype(std::declval<ContainerType<ValueType, Args...>>().cend())>,
    void>
  > : public std::true_type {};

TEST(EnableIf_Test, IsStlLikeContainer)
{
  using namespace std;
  EXPECT_EQ(true, is_stl_like_container<vector<int>>::value);
  EXPECT_EQ(true, is_stl_like_container<list<int>>::value);
  EXPECT_EQ(true, is_stl_like_container<deque<int>>::value);
  EXPECT_EQ(true, is_stl_like_container<set<int>>::value);

  auto m = is_stl_like_container<map<int, int>>::value;
  EXPECT_EQ(true, m);
  m = is_stl_like_container<map<int, int>>::value;
//  m = is_stl_like_container<array<int, 1>>::value; // returning false
  EXPECT_EQ(true, m);

  // this is not compiling -> 
  // error: macro "EXPECT_EQ" passed 3 arguments, but takes just 2
  // error: ‘EXPECT_EQ’ was not declared in this scope
  //EXPECT_EQ(true, is_stl_like_container<multimap<int, int>>::value);

  EXPECT_EQ(false, is_stl_like_container<int>::value);
}

//specialize a type for all of the STL containers.
namespace is_stl_container_impl{
  template <typename T>       struct is_stl_container:std::false_type{};
  template <typename T, std::size_t N> struct is_stl_container<std::array    <T,N>>    :std::true_type{};
  template <typename... Args> struct is_stl_container<std::vector            <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::deque             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::list              <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::forward_list      <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::set               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::multiset          <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::map               <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::multimap          <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::stack             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::queue             <Args...>>:std::true_type{};
  template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>>:std::true_type{};
}

//type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
  static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};


TEST(EnableIf_Test, IsStlContainer)
{
  using namespace std;
  EXPECT_EQ(true, is_stl_container<vector<int>>::value);
  EXPECT_EQ(true, is_stl_container<list<int>>::value);
  EXPECT_EQ(true, is_stl_container<deque<int>>::value);
  EXPECT_EQ(true, is_stl_container<set<int>>::value);

  auto m = is_stl_container<map<int, int>>::value;
  EXPECT_EQ(true, m);
  m = is_stl_container<array<int, 1>>::value;
  EXPECT_EQ(true, m);

  // this is not compiling -> 
  // error: macro "EXPECT_EQ" passed 3 arguments, but takes just 2
  // error: ‘EXPECT_EQ’ was not declared in this scope
  // EXPECT_EQ(true, is_stl_container<multimap<int, int>>::value);

  EXPECT_EQ(false, is_stl_container<int>::value);
}

}
}
}