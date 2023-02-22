/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE.TXT for more information
 */

#ifndef __UTIL_H
#define __UTIL_H

unsigned char lsb(unsigned short word);
unsigned char msb(unsigned short word);
unsigned short unsigned_word(unsigned char msb, unsigned char lsb);
signed short signed_word(unsigned char msb, unsigned char lsb);

#define BT(var, bit) ((var) & (1 << (bit)))

#endif /* __UTIL_H */
