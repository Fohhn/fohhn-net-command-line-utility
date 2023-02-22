/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE.TXT for more information
 */

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

unsigned char lsb(unsigned short word)
{
  return word & 0xFF;
}

unsigned char msb(unsigned short word)
{
  return (word >> 8) & 0xFF;
}

unsigned short unsigned_word(unsigned char msb, unsigned char lsb)
{
  return (unsigned short)((msb << 8) + lsb);
}

signed short signed_word(unsigned char msb, unsigned char lsb)
{
  return (signed short)((msb << 8) + lsb);
}
