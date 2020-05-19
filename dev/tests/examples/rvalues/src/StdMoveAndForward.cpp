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

class SomeDataClass
{
 public:
  SomeDataClass()
      :
      data_(nullptr),
      length_(0)
  {
  }

  SomeDataClass(const SomeDataClass &rhs)
  {
    if (this != &rhs)
    {
      if (rhs.data_ != nullptr & rhs.length_ > 0)
      {
        length_ = rhs.length_;
        data_ = new char[length_ + 1];
        std::memcpy(data_, rhs.data_, length_);
      }
      else
      {
        data_ = nullptr;
        length_ = 0;
      }
    }
  }

  SomeDataClass(SomeDataClass &&rhs)
  {
    if (this != &rhs)
    {
      *this = std::move(rhs);
    }
  }

  ~SomeDataClass()
  {
    if (data_)
    {
      delete[] data_;
    }
    length_ = 0;
  }

  SomeDataClass& operator=(const SomeDataClass &rhs)
  {
    if (this != &rhs)
    {
      SomeDataClass tmp(rhs);
      std::swap(data_, tmp.data_);
      std::swap(length_, tmp.length_);
    }
    return *this;
  }

  SomeDataClass& operator=(SomeDataClass &&rhs)
  {
    if (this != &rhs)
    {
      data_ = rhs.data_;  // only "stealing" the data_ pointer
                          // no need for copying the data
      length_ = rhs.length_;

      rhs.data_ = nullptr;
      rhs.length_ = 0;
    }
    return *this;
  }

  char *data_;
  size_t length_;
};

class Widget
{
 public:
  using MyCounter = CtorCounter<std::string>;

  Widget() = default;

  Widget(const Widget &rhs)
      :
      counter_(rhs.counter_),
      data_(rhs.data_)
  {
  }

  Widget(Widget &&rhs)             // rhs definitely refers to
      ://                          // an object eligible for moving
      counter_(std::move(rhs.counter_)),
      data_(std::move(rhs.data_))
  {
  }

  virtual ~Widget()
  {
  }

  /*
   * This will forward "new_counter" into "counter_", in order words,
   * if set_data_forward is invoked with an lvalue,
   *     it'll copy "new_counter" to "counter_"
   * if set_data_forward is invoked with an rvalue,
   *     it'll move "new_counter" into "counter_"
   */
  template<typename T>
  void set_counter_forward(T &&new_counter)  // new_counter is universal reference
  {
    counter_ = std::forward<T>(new_counter);
  }

  /*
   * This will forward "new_data" into "data_", in order words,
   * if set_data_forward is invoked with an lvalue,
   *     it'll copy "new_data" to "data_"
   * if set_data_forward is invoked with an rvalue,
   *     it'll move "new_data" into "data_"
   */
  template<typename T>
  void set_data_forward(T &&new_data)        // new_data is universal reference
  {
    data_ = std::forward<T>(new_data);
  }

  template<typename T>
  void set_counter_move(T &&new_counter)     // universal reference
  {                                          // compiles, but is bad, bad, bad!
    counter_ = std::move(new_counter);
  }

  template<typename T>
  void set_data_move(T &&new_data)           // universal reference
  {                                          // compiles, but is bad, bad, bad!
    data_ = std::move(new_data);
  }

  void set_counter(const MyCounter &new_counter)  // set from lvalue
  {
    counter_ = new_counter;
  }

  void set_data(const SomeDataClass &new_data)    // set from const lvalue
  {
    data_ = new_data;
  }

  void set_counter(MyCounter &&new_counter)  // set from rvalue
  {
    counter_ = std::move(new_counter);
  }
  void set_data(SomeDataClass &&new_data)    // set from rvalue
  {
    data_ = std::move(new_data);
  }

  MyCounter counter_;
  SomeDataClass data_;
};

TEST(StdMoveAndForward_Test, SetDataTemplateMove)
{
  char *data_ptr = new char[11];

  SomeDataClass data;
  data.data_ = data_ptr;
  data.length_ = 10;

  const char *kTestName = "Jefferson";
  strcpy(data.data_, kTestName);

  Widget widget;
  // this is expected to move "data" into "widget.data_", indeed
  widget.set_data_move(data);

  EXPECT_EQ(widget.data_.data_, data_ptr);  // widget.data_.data_ has been "moved"
  EXPECT_EQ(widget.data_.length_, 10);     // widget.data_.length "moved" too
  EXPECT_EQ(strcmp(widget.data_.data_, kTestName), 0);  // just to be sure that
                                                        // the contents haven't changed

  // "data" shall already be "unknown" at this point
  EXPECT_EQ(data.data_, nullptr);
  EXPECT_EQ(data.length_, 0);
}

TEST(StdMoveAndForward_Test, SetDataTemplateForwardLvalue)
{
  char *data_ptr = new char[11];

  SomeDataClass data;
  data.data_ = data_ptr;
  data.length_ = 10;

  const char *kTestName = "Jefferson";
  strcpy(data.data_, kTestName);

  Widget widget;
  // this is expected to create a copy of "data" into "widget.data_"
  widget.set_data_forward(data);

  EXPECT_NE(widget.data_.data_, data_ptr);  // "data_" pointers shall be different
                                            // once a new copy has been created
  EXPECT_EQ(widget.data_.length_, 10);
  EXPECT_EQ(strcmp(widget.data_.data_, kTestName), 0);  // the contents shall be equal, though

  // "data" shall be the same, no changes is expected
  EXPECT_EQ(data.data_, data_ptr);
  EXPECT_EQ(data.length_, 10);
}

TEST(StdMoveAndForward_Test, SetDataTemplateForwardRvalue)
{
  char *data_ptr = new char[11];

  SomeDataClass data;
  data.data_ = data_ptr;
  data.length_ = 10;

  const char *kTestName = "Jefferson";
  strcpy(data.data_, kTestName);

  Widget widget;
  // this time, it is expected to move "data" into "widget.data_"
  widget.set_data_forward(std::move(data));

  EXPECT_EQ(widget.data_.data_, data_ptr);  // widget.data_.data_ has been "moved"
  EXPECT_EQ(widget.data_.length_, 10);     // widget.data_.length "moved" too
  EXPECT_EQ(strcmp(widget.data_.data_, kTestName), 0);  // just to be sure that
                                                        // the contents haven't changed

  // "data" shall already be "unknown" at this point
  EXPECT_EQ(data.data_, nullptr);
  EXPECT_EQ(data.length_, 0);
}

// TODO create test cases to demonstrate the pros and cons of:
//      set_data(const SomeDataClass&), set_data(SomeDataClass&&)

class PerfectFwTest : public ::testing::Test
{
 public:
  using MyIntCounter = CtorCounter<int>;

 private:
  void SetUp()
  {
    MyIntCounter::normalctor_counter_ = 0;
    MyIntCounter::copyctor_counter_ = 0;
    MyIntCounter::movector_counter_ = 0;
    MyIntCounter::dtor_counter_ = 0;
  }
};

template<typename T>
struct MyWrapper
{
  MyWrapper() = delete;

  template<typename Other>
  MyWrapper(Other &&arg)
      :
      data_(std::forward<Other>(arg))
  {
  }

  MyWrapper(const MyWrapper&) = delete;
  MyWrapper(const MyWrapper&&) = delete;

  T data_;
};

TEST_F(PerfectFwTest, perfect_fw_rvalue)
{
  {
    MyWrapper<MyIntCounter> uniref(10);

    EXPECT_EQ(MyIntCounter::dtor_counter_, 0);
  }

  EXPECT_EQ(MyIntCounter::normalctor_counter_, 1);
  EXPECT_EQ(MyIntCounter::copyctor_counter_, 0);
  EXPECT_EQ(MyIntCounter::movector_counter_, 0);
  EXPECT_EQ(MyIntCounter::dtor_counter_, 1);
}

TEST_F(PerfectFwTest, perfect_fw_move)
{
  {
    MyIntCounter my_uniq(10);
    MyWrapper<MyIntCounter> uniref(std::move(my_uniq));

    EXPECT_EQ(MyIntCounter::dtor_counter_, 0);
  }

  EXPECT_EQ(MyIntCounter::normalctor_counter_, 1);
  EXPECT_EQ(MyIntCounter::movector_counter_, 1);
  EXPECT_EQ(MyIntCounter::copyctor_counter_, 0);
  EXPECT_EQ(MyIntCounter::dtor_counter_, 2);
}

TEST_F(PerfectFwTest, perfect_fw_lvalue)
{
  {
    MyIntCounter my_uniq(10);
    MyWrapper<MyIntCounter> uniref(my_uniq);

    EXPECT_EQ(MyIntCounter::dtor_counter_, 0);
  }

  EXPECT_EQ(MyIntCounter::copyctor_counter_, 1);
  EXPECT_EQ(MyIntCounter::normalctor_counter_, 1);
  EXPECT_EQ(MyIntCounter::movector_counter_, 0);
  EXPECT_EQ(MyIntCounter::dtor_counter_, 2);
}

}
}
}

