#include <utility>

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

}
}
}