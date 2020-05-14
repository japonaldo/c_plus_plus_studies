#include <gtest/gtest.h>

#include <string>

namespace jmf
{
namespace tests
{
namespace examples
{

struct MyStruct
{
  int a;
  std::string str;
};

void Foo(MyStruct &my)
{
  my.a = 0xdeadbeef;
  my.str = "lvalue reference";
}

void Foo(MyStruct &&my)
{
  my.a = 0xdeadbeef;
  my.str = "rvalue reference";
}

TEST(RvalueRef_Test, LvalueRef)
{
  MyStruct my;
  Foo(my);

  EXPECT_EQ(my.a, 0xdeadbeef);
  EXPECT_EQ(my.str, "lvalue reference");
}

TEST(RvalueRef_Test, RvalueRef)
{
  MyStruct my;
  Foo(std::move(my));

  EXPECT_EQ(my.a, 0xdeadbeef);
  EXPECT_EQ(my.str, "rvalue reference");
}

}
}
}
