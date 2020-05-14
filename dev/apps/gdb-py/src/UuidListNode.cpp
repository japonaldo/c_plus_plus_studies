// TODO add Copyright

#include "UuidListNode.h"

#include <assert.h>

extern jmf::apps::gdbpy::UuidListNode *g_list_head;

namespace jmf
{
namespace apps
{
namespace gdbpy
{

void ListAddUuid(const Uuid *uuid)
{
  UuidListNode *node_to_add = new UuidListNode;
  assert(node_to_add != NULL);

  node_to_add->uuid = *uuid;

// Add entry to the front of the list
  node_to_add->next = g_list_head;
  g_list_head = node_to_add;
}

//
// Code to populate the list with fake data
//

// A completely fake uuid-generator implementation
void GenerateFakeUuid(Uuid *uuid)
{
  for (size_t i = 0; i < sizeof(*uuid); i++)
  {
    uuid->bytes[i] = rand() & 0xff;
  }
}

void GenerateFakeUuidList()
{
  for (int i = 0; i < 10; i++)
  {
    Uuid uuid;
    GenerateFakeUuid(&uuid);
    ListAddUuid(&uuid);
  }
}

}
}
}
