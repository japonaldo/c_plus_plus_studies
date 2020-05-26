#include "TemplateInheritance.h"

#include <stddef.h>

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <utility>

#include <gtest/gtest.h>


namespace jmf
{
namespace tests
{
namespace examples
{

TEST(TemplateInheritance_Test, MyBaseTemplateInt)
{
  MyBaseTemplate<int> base;

  base.set(10);
  EXPECT_EQ(base.get(), 10);
}

TEST(TemplateInheritance_Test, MyDerivedTemplateInt)
{
  MyDerivedTemplate<int, float> derived{10, 20.5};

  EXPECT_EQ(derived.get(), 10);

  derived.set(50);
  EXPECT_EQ(derived.get(), 500);
}

}
}
}
