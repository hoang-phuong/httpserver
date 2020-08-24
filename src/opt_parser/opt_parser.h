#ifndef OPT_PARSER_H
#define OPT_PARSER_H

#include "std_types.h"

/* ** Defines ****************************************************************/
#define OPTPARSER_MAX_NUM_OPTS            10
#define OPTPARSER_LONG_NAME_LENGTH        10
#define OPTPARSER_OPTION_VALUE_LENGTH     4096

/* ** Type definitions *******************************************************/
typedef struct 
{
  char          shortName;
  char          longName[OPTPARSER_LONG_NAME_LENGTH];
  char          value[OPTPARSER_OPTION_VALUE_LENGTH];
  int           isParsed;
} OptParser_OptionT;

/* ** Public interfaces ******************************************************/
int OptParser_AddOption(char shortName, const char* longName);

int OptParser_Read(const char* option, char* value);

int OptParser_Process(int argc, char* argv[]);


#endif /* #ifndef OPT_PARSER_H */