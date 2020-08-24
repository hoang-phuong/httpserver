/* ** Includes ***************************************************************/
#include "opt_parser.h"
#include <string.h>
#include <stdio.h>

/* ** Static variables   *****************************************************/
static OptParser_OptionT OptParser_OptionList[OPTPARSER_MAX_NUM_OPTS];
static unsigned char     numOptions = 0;

/* ** Static implementations   ***********************************************/
static int checkShortName(const char* s)
{
  if ((strlen(s) != 2) || ('-' != s[0]))
  {
    return -1;
  }

  int i;
  OptParser_OptionT* pOption;

  for (i = 0; i < OPTPARSER_MAX_NUM_OPTS; i++)
  {
    pOption = &OptParser_OptionList[i];

    if (s[1] != pOption->shortName)
    {
      continue;
    }

    return i;
  }

  return -1;

}

static int checkLongName(const char* s)
{
  if ((strlen(s) < 3) || ('-' != s[0]) || ('-' != s[1]))
  {
    return -1;
  }

  int i;
  OptParser_OptionT* pOption;

  for (i = 0; i < OPTPARSER_MAX_NUM_OPTS; i++)
  {
    pOption = &OptParser_OptionList[i];

    if (0 != strcmp(&s[2], pOption->longName))
    {
      continue;
    }

    return i;
  }

  return -1;
}


/* ** Public implementations *************************************************/
int OptParser_AddOption(char shortName, const char* longName)
{
  if (numOptions >= OPTPARSER_MAX_NUM_OPTS)
  {
    fprintf(stderr, "Number of options reach the limit (%d)", OPTPARSER_MAX_NUM_OPTS);
    return 1;
  }
  OptParser_OptionT *pOption = &OptParser_OptionList[numOptions];

  pOption->shortName = shortName;
  strcpy(pOption->longName, longName);
  pOption->isParsed = 0;
  numOptions++;

  return 0;
}

int OptParser_Process(int argc, char* argv[])
{
  int i;
  int valueExpected = 0;
  int idx           = -1;

  /* Skip executable name */
  for (i = 1; i < argc; i++)
  {
    if (0 == valueExpected)
    {
      idx = checkLongName(argv[i]);

      if ((idx < 0) || ((i + 1) == argc))
      {
        idx = checkShortName(argv[i]);
      }

      if (idx < 0)
      {
        fprintf(stderr, "Error occured while parsing options");
        return 1;
      }

      /* Option name is valid */
      valueExpected = 1;
    }
    else
    {
      /* Copy data into option value buffer */
      OptParser_OptionT* pOption = &OptParser_OptionList[idx];

      strcpy(pOption->value, argv[i]);

      pOption->isParsed = 1;

      valueExpected = 0;
    }
  }

  return 0;
}

int OptParser_Read(const char* option, char* value)
{
  int idx;

  idx = checkShortName(option);

  if (idx < 0)
  {
    idx = checkLongName(option);
  }

  if (idx < 0)
  {
    fprintf(stderr, "Option %s is not expected", option);
    return -1;
  }

  OptParser_OptionT* pOption = &OptParser_OptionList[idx];

  strcpy(value, pOption->value);

  return 0;
}