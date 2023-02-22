/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE.TXT for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "cmd.h"
#include "util.h"

unsigned char cmd_buf[CMD_BUF_LEN];

int cmd_buf_pos;

int cmd_begin()
{
  cmd_buf_pos = 1;
  cmd_buf[0] = START_BYTE;
}

int cmd_add_byte(unsigned char d)
{
  if (d == START_BYTE)
  {
    cmd_buf[cmd_buf_pos] = CONTROL_BYTE;
    cmd_buf_pos++;
    cmd_buf[cmd_buf_pos] = 0x00;
  }
  else if (d == CONTROL_BYTE)
  {
    cmd_buf[cmd_buf_pos] = CONTROL_BYTE;
    cmd_buf_pos++;
    cmd_buf[cmd_buf_pos] = 0x01;
  }
  else
  {
    cmd_buf[cmd_buf_pos] = d;
  }
  cmd_buf_pos++;
}

int cmd_load_preset(int id, int nr)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);               /* data byte count */
  cmd_add_byte(CMD_LOAD_PRESET); /* command */
  cmd_add_byte(1);               /* msb */
  cmd_add_byte(nr);              /* lsb */
  cmd_add_byte(0);               /* data */
  return cmd_buf_pos;
}

int cmd_set_speaker(int id, int channel, int nr)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);               /* data byte count */
  cmd_add_byte(CMD_SET_SPEAKER); /* command */
  cmd_add_byte(channel);         /* msb */
  cmd_add_byte(nr);              /* lsb */
  cmd_add_byte(0);               /* data */
  return cmd_buf_pos;
}

int cmd_get_preset(int id)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_GET_PRESET_NAME);
  cmd_add_byte(1);
  cmd_add_byte(0);
  cmd_add_byte(0);
  return cmd_buf_pos;
}

int cmd_get_speaker(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_GET_SPEAKER);
  cmd_add_byte(channel);
  cmd_add_byte(0);
  cmd_add_byte(0);
  return cmd_buf_pos;
}

int cmd_set_vol(int id, int channel, int vol, bool on, bool inv)
{
  int flags = 0;
  if (on)
    flags += 1;
  if (inv)
    flags += 2;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_VOL);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(vol));
  cmd_add_byte(lsb(vol));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_rvol(int id, int channel, int vol)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_RVOL);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(vol));
  cmd_add_byte(lsb(vol));
  cmd_add_byte(1);
  return cmd_buf_pos;
}

int cmd_get_vol(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_VOL);
  return cmd_buf_pos;
}

int cmd_set_mute(int id, int channel, bool on)
{
  int flags = 0;
  if (on)
    flags += 5;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_RVOL);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_get_mute(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_VOL);
  return cmd_buf_pos;
}

int cmd_set_standby(int id, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_STANDBY);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_route(int id, int out_ch, int in_ch, int vol, bool on, bool inv)
{
  int flags = 0;
  if (on)
    flags += 1;
  if (inv)
    flags += 2;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_ROUTE);
  cmd_add_byte(out_ch);
  cmd_add_byte(in_ch);
  cmd_add_byte(msb(vol));
  cmd_add_byte(lsb(vol));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_get_route(int id, int out_ch, int in_ch)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(out_ch);
  cmd_add_byte(in_ch);
  cmd_add_byte(CMD_SET_ROUTE);
  return cmd_buf_pos;
}

int cmd_get_standby(int id)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(CMD_SET_STANDBY);
  return cmd_buf_pos;
}

int cmd_get_info(int id)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_GET_INFO);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(1);
  return cmd_buf_pos;
}

int cmd_get_controls(int id)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_GET_CONTROLS);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(1);
  return cmd_buf_pos;
}

int cmd_set_light(int id, bool on, bool sign)
{
  int flags = 0;
  if (on)
    flags += 1;
  if (sign)
    flags += 2;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_LIGHT);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_eq(int id, int channel, int nr, int freq, int q, int gain, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(7);
  cmd_add_byte(CMD_SET_EQ);
  cmd_add_byte(channel);
  cmd_add_byte(nr);
  cmd_add_byte(msb(freq));
  cmd_add_byte(lsb(freq));
  cmd_add_byte(msb(q));
  cmd_add_byte(lsb(q));
  cmd_add_byte(msb(gain));
  cmd_add_byte(lsb(gain));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_xover(int id, int channel, int nr, int freq, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(4);
  cmd_add_byte(CMD_SET_XOVER);
  cmd_add_byte(channel);
  cmd_add_byte(nr);
  cmd_add_byte(msb(freq));
  cmd_add_byte(lsb(freq));
  cmd_add_byte(nr);
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_delay(int id, int channel, int time, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_DELAY);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(time));
  cmd_add_byte(lsb(time));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_gate(int id, int channel, int thrs, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_GATE);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(thrs));
  cmd_add_byte(lsb(thrs));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_gate_time(int id, int channel, int time)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(2);
  cmd_add_byte(CMD_SET_GATE_TIME);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(time));
  cmd_add_byte(lsb(time));
  return cmd_buf_pos;
}

int cmd_set_dynamic(int id, int channel, int lim, int comp, int ratio, bool on)
{
  int flags = 0;
  if (on)
    flags += 1;
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(7);
  cmd_add_byte(CMD_SET_DYNAMIC);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(lim));
  cmd_add_byte(lsb(lim));
  cmd_add_byte(msb(comp));
  cmd_add_byte(lsb(comp));
  cmd_add_byte(msb(ratio));
  cmd_add_byte(lsb(ratio));
  cmd_add_byte(flags);
  return cmd_buf_pos;
}

int cmd_set_dyn_gain(int id, int channel, int gain)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(3);
  cmd_add_byte(CMD_SET_DYNAMIC_GAIN);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(gain));
  cmd_add_byte(lsb(gain));
  cmd_add_byte(1);
  return cmd_buf_pos;
}

int cmd_set_dyn_time(int id, int channel, int att, int rel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(4);
  cmd_add_byte(CMD_SET_DYNAMIC_TIME);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(msb(att));
  cmd_add_byte(lsb(att));
  cmd_add_byte(msb(rel));
  cmd_add_byte(lsb(rel));
  return cmd_buf_pos;
}

int cmd_get_light(int id)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(0);
  cmd_add_byte(0);
  cmd_add_byte(CMD_SET_LIGHT);
  return cmd_buf_pos;
}

int cmd_get_eq(int id, int channel, int nr)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(nr);
  cmd_add_byte(CMD_SET_EQ);
  return cmd_buf_pos;
}

int cmd_get_xover(int id, int channel, int nr)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(nr);
  cmd_add_byte(CMD_SET_XOVER);
  return cmd_buf_pos;
}

int cmd_get_delay(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_DELAY);
  return cmd_buf_pos;
}

int cmd_get_gate(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_GATE);
  return cmd_buf_pos;
}

int cmd_get_gate_time(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_GATE_TIME);
  return cmd_buf_pos;
}

int cmd_get_dynamic(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_DYNAMIC);
  return cmd_buf_pos;
}

int cmd_get_dyn_gain(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_DYNAMIC_GAIN);
  return cmd_buf_pos;
}

int cmd_get_dyn_time(int id, int channel)
{
  cmd_begin();
  cmd_add_byte(id);
  cmd_add_byte(1);
  cmd_add_byte(CMD_READBACK);
  cmd_add_byte(channel);
  cmd_add_byte(1);
  cmd_add_byte(CMD_SET_DYNAMIC_TIME);
  return cmd_buf_pos;
}
