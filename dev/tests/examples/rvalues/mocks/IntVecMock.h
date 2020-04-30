#include <gmock/gmock.h>

#include "IntVec.h"

#include <iostream>
using namespace std;

namespace jmf
{
namespace tests
{
namespace examples
{

class IntVecMock : public IntVec
{
 public:
  explicit IntVecMock(size_t num = 0)
      :
      IntVec(num)
  {
  }

  virtual ~IntVecMock()
  {
  }

  MOCK_METHOD(void, Log, (const char *), (override));
};

}
}
}
