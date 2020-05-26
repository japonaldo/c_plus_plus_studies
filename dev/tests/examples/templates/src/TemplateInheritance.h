#ifndef DEV_TESTS_EXAMPLES_TEMPLATES_SRC_TEMPLATEINHERITANCE_H_
#define DEV_TESTS_EXAMPLES_TEMPLATES_SRC_TEMPLATEINHERITANCE_H_

namespace jmf
{
namespace tests
{
namespace examples
{

template<typename T>
class MyBaseTemplate
{
  public:
    MyBaseTemplate() = default;
    MyBaseTemplate(const T& v);

    virtual void set(const T& v);
    virtual T get() const;

    virtual ~MyBaseTemplate();

  protected:
    T value_;
};

template<typename T1, typename T2>
class MyDerivedTemplate : public MyBaseTemplate<T1>
{
 public:
  MyDerivedTemplate() = default;
  MyDerivedTemplate(const T1& v1, const T2& v2);

  void set(const T1& v) override;
  T1 get() const override;

 private:
  T2 derived_value_;
};

}
}
}
#include "TemplateInheritanceImpl.h"


#endif /* DEV_TESTS_EXAMPLES_TEMPLATES_SRC_TEMPLATEINHERITANCE_H_ */
