// TODO add Copyright

#include "gtest/gtest.h"
#include "Foo.h"

TEST(FooTest, SumTest) {
    EXPECT_EQ (Foo::Sum(2, 3), 5);
    EXPECT_EQ (Foo::Sum(1, 1), 2);
    EXPECT_EQ (Foo::Sum(4, 2), 6);
}
