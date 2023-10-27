/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "decode.h"
#include "util.h"

uint8_t rx_buf[RX_BUF_LEN];
uint8_t reply_buf[REPLY_BUF_LEN];

int reply_id;
int reply_pos;
bool cb_received;

#define END_BYTE 0xF0
#define DECODE_BYTE 0xFF

void decode_begin(int id)
{
  reply_id = id;
  reply_pos = 0;
  cb_received = false;
}

int decode_rx_data(uint8_t *raw, int len)
{
  int i = 0;
  for (int i = 0; i < len; i++)
  {
    if (reply_pos < REPLY_BUF_LEN)
    {
      if (cb_received)
      {
        cb_received = false;
        if (rx_buf[i] == 0x00)
        {
          reply_buf[reply_pos] = 0xF0;
          reply_pos++;
        }
        else if (rx_buf[i] == 0x01)
        {
          reply_buf[reply_pos] = 0xFF;
          reply_pos++;
        }
        else
        {
          return -1;
        }
      }
      else if (rx_buf[i] == DECODE_BYTE)
      {
        cb_received = true;
      }
      else if (rx_buf[i] == END_BYTE)
      {
        if (reply_pos > 0)
        {
          if (reply_buf[reply_pos - 1] == reply_id)
          {
            return reply_pos - 1;
          }
        }
      }
      else
      {
        reply_buf[reply_pos] = rx_buf[i];
        reply_pos++;
      }
    }
  }
  return -1;
}

int get_preset(int *nr, char *name, int num)
{
  if (num > 17)
    return 1;
  *nr = reply_buf[0];
  strncpy(name, reply_buf + 2, num);
  name[num - 1] = '\0';
  return 0;
}

int get_speaker(int *nr, char *name, int num)
{
  if (num > 17)
    return 1;
  *nr = reply_buf[0];
  strncpy(name, reply_buf + 2, num);
  name[num - 1] = '\0';
  return 0;
}

int get_gain(int16_t *gain, bool *on, bool *inv)
{
  *gain = signed_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[2], 0);
  *inv = BT(reply_buf[2], 1);
  return 0;
}

int get_mute(bool *on)
{
  *on = BT(reply_buf[2], 0);
  return 0;
}

int get_standby(bool *on)
{
  if (BT(reply_buf[0], 0))
    *on = true;
  else
    *on = false;
  return 0;
}

int get_info(uint16_t *device_class, uint8_t *version)
{
  *device_class = unsigned_word(reply_buf[0], reply_buf[1]);
  version[0] = reply_buf[2];
  version[1] = reply_buf[3];
  version[2] = reply_buf[4];
  return 0;
}

int get_temp(short *temp)
{
  *temp = signed_word(reply_buf[1], reply_buf[2]);
  return 0;
}

int get_stat(uint8_t *stat)
{
  *stat = reply_buf[0];
  return 0;
}

int get_light(bool *on, bool *sign)
{
  if (BT(reply_buf[0], 0))
    *on = true;
  else
    *on = false;
  return 0;
  if (BT(reply_buf[0], 1))
    *sign = true;
  else
    *sign = false;
  return 0;
}

int get_eq(uint16_t *freq, uint16_t *q, int16_t *gain, bool *on)
{
  *freq = unsigned_word(reply_buf[0], reply_buf[1]);
  *q = unsigned_word(reply_buf[2], reply_buf[3]);
  *gain = signed_word(reply_buf[4], reply_buf[5]);
  *on = BT(reply_buf[6], 0);
  return 0;
}

int get_xover(int16_t *freq, bool *on)
{
  *freq = unsigned_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[3], 0);
  return 0;
}

int get_time(uint16_t *time, bool *on)
{
  *time = unsigned_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[2], 0);
  return 0;
}

int get_gate(int16_t *thrs, bool *on)
{
  *thrs = signed_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[2], 0);
  return 0;
}

int get_gate_time(uint16_t *time)
{
  *time = unsigned_word(reply_buf[0], reply_buf[1]);
  return 0;
}

int get_dynamic(int16_t *lim, int16_t *comp, int16_t *ratio, bool *on)
{
  *lim = signed_word(reply_buf[0], reply_buf[1]);
  *comp = signed_word(reply_buf[2], reply_buf[3]);
  *ratio = signed_word(reply_buf[4], reply_buf[5]);
  *on = BT(reply_buf[6], 0);
  return 0;
}

int get_dyn_gain(int16_t *gain)
{
  *gain = signed_word(reply_buf[0], reply_buf[1]);
  return 0;
}

int get_dyn_time(int16_t *att, int16_t *rel)
{
  *att = unsigned_word(reply_buf[0], reply_buf[1]);
  *rel = unsigned_word(reply_buf[2], reply_buf[3]);
  return 0;
}
