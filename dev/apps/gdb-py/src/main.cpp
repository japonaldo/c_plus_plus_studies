// TODO add Copyright

#include <iostream>

#include "UuidListNode.h"

using jmf::apps::gdbpy::UuidListNode;
extern UuidListNode *g_list_head;
UuidListNode *g_list_head = nullptr;

int main()
{
  std::cout << "Hello World, this is Fujioka!" << std::endl;
  jmf::apps::gdbpy::GenerateFakeUuidList();
  // Let's just spin loop so we can break at any point and dump the list
  while (1)
  {
  }

  return 0;
}
