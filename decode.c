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

#include "decode.h"
#include "util.h"

unsigned char rx_buf[RX_BUF_LEN];
unsigned char reply_buf[REPLY_BUF_LEN];

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

int decode_rx_data(unsigned char *raw, int len)
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

int get_gain(signed short *gain, bool *on, bool *inv)
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

int get_info(unsigned short *device_class, unsigned char *version)
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

int get_stat(unsigned char *stat)
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

int get_eq(unsigned short *freq, unsigned short *q, signed short *gain, bool *on)
{
  *freq = unsigned_word(reply_buf[0], reply_buf[1]);
  *q = unsigned_word(reply_buf[2], reply_buf[3]);
  *gain = signed_word(reply_buf[4], reply_buf[5]);
  *on = BT(reply_buf[6], 0);
  return 0;
}

int get_xover(signed short *freq, bool *on)
{
  *freq = unsigned_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[3], 0);
  return 0;
}

int get_time(unsigned short *time, bool *on)
{
  *time = unsigned_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[2], 0);
  return 0;
}

int get_gate(signed short *thrs, bool *on)
{
  *thrs = signed_word(reply_buf[0], reply_buf[1]);
  *on = BT(reply_buf[2], 0);
  return 0;
}

int get_gate_time(unsigned short *time)
{
  *time = unsigned_word(reply_buf[0], reply_buf[1]);
  return 0;
}

int get_dynamic(signed short *lim, signed short *comp, signed short *ratio, bool *on)
{
  *lim = signed_word(reply_buf[0], reply_buf[1]);
  *comp = signed_word(reply_buf[2], reply_buf[3]);
  *ratio = signed_word(reply_buf[4], reply_buf[5]);
  *on = BT(reply_buf[6], 0);
  return 0;
}

int get_dyn_gain(signed short *gain)
{
  *gain = signed_word(reply_buf[0], reply_buf[1]);
  return 0;
}

int get_dyn_time(signed short *att, signed short *rel)
{
  *att = unsigned_word(reply_buf[0], reply_buf[1]);
  *rel = unsigned_word(reply_buf[2], reply_buf[3]);
  return 0;
}
