/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util.h"

uint8_t lsb(uint16_t word)
{
  return word & 0xFF;
}

uint8_t msb(uint16_t word)
{
  return (word >> 8) & 0xFF;
}

uint16_t unsigned_word(uint8_t msb, uint8_t lsb)
{
  return (uint16_t)((msb << 8) + lsb);
}

int16_t signed_word(uint8_t msb, uint8_t lsb)
{
  return (int16_t)((msb << 8) + lsb);
}
