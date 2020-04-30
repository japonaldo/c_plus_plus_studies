#ifndef DEV_TESTS_EXAMPLES_RVALUES_SRC_CTORCOUNTER_H_
#define DEV_TESTS_EXAMPLES_RVALUES_SRC_CTORCOUNTER_H_

#include <utility>

namespace jmf
{
namespace tests
{
namespace examples
{

template<typename T>
class CtorCounter
{
 public:
  CtorCounter()
  {
  }

  CtorCounter(T &&val)
      :
      data_(std::forward<T>(val))
  {
  }

  CtorCounter(const CtorCounter &rhs)
  {
    ++copyctor_counter_;
    if (this != &rhs)
    {
      data_ = rhs.data_;
    }
  }

  CtorCounter(CtorCounter &&rhs)
  {
    ++movector_counter_;
    if (this != &rhs)
    {
      *this = std::move(rhs);
    }
  }

  virtual ~CtorCounter()
  {
    ++dtor_counter_;
  }

  CtorCounter& operator=(const CtorCounter &rhs)
  {
    if (this != &rhs)
    {
      CtorCounter tmp(rhs);
      std::swap(data_, tmp.data_);
    }
    return *this;
  }

  CtorCounter& operator=(CtorCounter &&rhs)
  {
    if (this != &rhs)
    {
      data_ = std::move(rhs.data_);
    }
    return *this;
  }

  static int movector_counter_;
  static int copyctor_counter_;
  static int dtor_counter_;

  // it's public to facilitate the tests
  T data_;
};

template<typename T>
int CtorCounter<T>::movector_counter_ = 0;

template<typename T>
int CtorCounter<T>::copyctor_counter_ = 0;

template<typename T>
int CtorCounter<T>::dtor_counter_ = 0;

}
}
}

#endif /* DEV_TESTS_EXAMPLES_RVALUES_SRC_CTORCOUNTER_H_ */
