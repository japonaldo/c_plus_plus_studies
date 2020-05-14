// TODO add Copyright

#ifndef DEV_APPS_GDB_PY_SRC_UUIDLISTNODE_H
#define DEV_APPS_GDB_PY_SRC_UUIDLISTNODE_H

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>


namespace jmf
{
namespace apps
{
namespace gdbpy
{

struct Uuid
{
  uint8_t bytes[16];
};

struct UuidListNode
{
  UuidListNode *next;
  Uuid uuid;
};


void ListAddUuid(const Uuid *uuid);

// A completely fake uuid-generator implementation
void GenerateFakeUuid(Uuid *uuid);

void GenerateFakeUuidList();

}
}
}

#endif //DEV_APPS_GDB_PY_SRC_UUIDLISTNODE_H
