/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>

uint8_t lsb(uint16_t word);
uint8_t msb(uint16_t word);
uint16_t unsigned_word(uint8_t msb, uint8_t lsb);
int16_t signed_word(uint8_t msb, uint8_t lsb);

#define BT(var, bit) ((var) & (1 << (bit)))

#endif /* __UTIL_H */
