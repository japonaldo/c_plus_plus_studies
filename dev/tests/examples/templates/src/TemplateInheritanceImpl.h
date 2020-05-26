#include "TemplateInheritance.h"

namespace jmf
{
namespace tests
{
namespace examples
{

template<typename T>
MyBaseTemplate<T>::MyBaseTemplate(const T &v)
    :
    value_(v)
{
}

template<typename T>
MyBaseTemplate<T>::~MyBaseTemplate()
{
}

template<typename T>
void MyBaseTemplate<T>::set(const T& v)
{
  value_ = v;
}

template<typename T>
T MyBaseTemplate<T>::get() const
{
  return value_;
}

template<typename T1, typename T2>
MyDerivedTemplate<T1, T2>::MyDerivedTemplate(const T1 &v1, const T2 &v2)
    :
    MyBaseTemplate<T1>(v1),
    derived_value_(v2)
{
}

template<typename T1, typename T2>
void MyDerivedTemplate<T1, T2>::set(const T1& v)
{
  // MyBaseTemplate<T1>::value_ = v * 10;
  this->value_ = v * 10;
}

template<typename T1, typename T2>
T1 MyDerivedTemplate<T1, T2>::get() const
{
  //return MyBaseTemplate<T1>::value_;
  return this->value_;
}

}
}
}
