#include "CtorCounter.h"

#include <vector>

#include <gtest/gtest.h>

namespace jmf
{
namespace tests
{
namespace examples
{

struct Widget
{
};

/*
 * A universal reference (T&&) is either rvalue reference or lvalue reference.
 * For a reference to be universal, type deduction is necessary, but it’s not sufficient.
 * It must be precisely “T&&”.
 */

//                           // rvalue reference
void f(Widget &&param)       // no type deduction
{
  Widget &&var1 = Widget();  // rvalue reference
                             // no type deduction

  auto &&var2 = var1;        // not rvalue reference
                             // type deduction
                             // var2 is a universal reference
}

//                           // rvalue reference
//                           // type deduction,
//                           // but the form is std::vector<T>, not T&&
template<typename T>
void f(std::vector<T> &&param)
{
}

//                           // rvalue reference
template<typename T>         // type deduction,
void f2(const T &&param)     // but the form is const T&&, not T&&
{
}

template<typename T>         // universal reference
void f(T &&param)            // type deduction of type T&&
{
}

void Example1()
{
  Widget w;

  f(w);             // lvalue passed to f; param's type is
                    // Widget& (i.e., an lvalue reference)
  f(std::move(w));  // rvalue passed to f; param's type is
                    // Widget&& (i.e., an rvalue reference)

}

void Example2()
{
  std::vector<int> v;
  //f(v);  // error! can't bind lvalue to rvalue reference
  //       // the form of param's type is std::vector<T>&&, not T&&
  //       // that's why it's not a universal reference
}

template<class T, class Allocator = std::allocator<T>>
class MyVector
{  // Standards
 public:
  /*
   * There is no type deduction. That’s because push_back can’t exist without a
   * particular MyVector instantiation for it to be part of,
   * and the type of that instantiation fully determines the declaration for push_back
   */
  void push_back(T &&x)      // rvalue reference
  {                          // no type deduction
  }

  /*
   * Parameter Args is independent of MyVector’s type parameter T,
   * so Args must be deduced each time emplace_back is called
   */
  template<class ... Args>
  void emplace_back(Args &&... args)  // universal reference
  {                                  // type deduction
  }
};

// MyVector<Widget> v; // that causes the MyVector template to be initialized as follows:
#if 0
class MyVector<Widget, allocator<Widget>>
{
 public:
  // even though at first look it might suggests that there is due to the form T&&
  // you can now see clearly that there is no type deduction for this method,
  void push_back(Widget &&x);
};
#endif

void Example3()
{
  auto timeFuncInvocation = [](auto &&func, auto &&... params)  // C++14
                               {
                                 // start timer;
                                 std::forward<decltype(func)>(func)(// invoke func
                                     std::forward<decltype(params)>(params)...// on params
                                 );
                                 // stop timer and record elapsed time;
                               };
}

/*
 * Lessons learned:
 * • If a function template parameter has type T&& for a deduced type T, or if an
 *   object is declared using auto&&, the parameter or object is a universal reference.
 * • If the form of the type declaration isn’t precisely type&&, or if type deduction
 *   does not occur, type&& denotes an rvalue reference.
 * • Universal references correspond to rvalue references if they’re initialized with
 *   rvalues. They correspond to lvalue references if they’re initialized with lvalues
 */

}
}
}

