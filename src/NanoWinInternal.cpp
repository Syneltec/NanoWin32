// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// Internal functions

#include "NanoWinInternal.h"

#define NanoWinFileNameMatchMaskMaxRecLevel    (32)
#define NanoWinFileNameMatchMaskSingleWildChar ('?')
#define NanoWinFileNameMatchMaskMultiWildChar  ('*')

template<typename C>
static bool NanoWinFileNameMatchMaskPartA(const C *name, const C *mask, unsigned int *recDepth)
{
  (*recDepth)++;

  if (*recDepth > NanoWinFileNameMatchMaskMaxRecLevel)
  {
    (*recDepth)--;

    return false; // Max recursion level reached
  }

  bool matchFlag = false;
  bool checkNextCharFlag = true; // is used to iterate through non-'*' mask characters without recursion

  while (checkNextCharFlag)
  {
    C nameChar = name[0];
    C maskChar = mask[0];

    checkNextCharFlag = false;

    if (maskChar == (C)('\0'))
    {
      matchFlag = nameChar == (C)('\0');
    }
    else if (maskChar == (C)(NanoWinFileNameMatchMaskMultiWildChar))
    {
      int offset = -1;

      do // try every possible offset until either match detection or end of string reached
      {
        offset++;

        matchFlag = NanoWinFileNameMatchMaskPartA(name + offset, mask + 1, recDepth);
      } while (!matchFlag && name[offset] != (C)('\0'));
    }
    else
    {
      if ((maskChar == nameChar) ||
        (maskChar == (C)(NanoWinFileNameMatchMaskSingleWildChar) && nameChar != (C)('\0')))
      {
        checkNextCharFlag = true; // check next chars without recursion

        name++;
        mask++;
      }
    }
  }

  (*recDepth)--;

  return matchFlag;
}

NW_EXTERN_C_BEGIN

extern bool NanoWinFileNameMatchMaskA(const char *name, const char *mask)
{
  unsigned int recDepth = 0;

  return NanoWinFileNameMatchMaskPartA(name, mask, &recDepth);
}

extern bool NanoWinFileNameMatchMaskW(const wchar_t *name, const wchar_t *mask)
{
  unsigned int recDepth = 0;

  return NanoWinFileNameMatchMaskPartA(name, mask, &recDepth);
}

NW_EXTERN_C_END