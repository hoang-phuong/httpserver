#include <stdio.h>
#include "std_types.h"
#include "opt_parser.h"
#include "linked_list.h"
#include "hash_table.h"

int main(int argc, char* argv[])
{
  printf("Hello World !\r\n");

  OptParser_AddOption('p', "port");
  OptParser_AddOption('s', "string");

  OptParser_Process(argc, argv);

  char value[4096] = {0};

  TcpServer_Start(8080, 5);

  return 0;
}
