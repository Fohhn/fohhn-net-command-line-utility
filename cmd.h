/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __CMD_H
#define __CMD_H

#define CMD_BUF_LEN 515

#define START_BYTE 0xF0
#define CONTROL_BYTE 0xFF

#define CMD_LOAD_PRESET 0x05
#define CMD_GET_PRESET_NAME 0x8E
#define CMD_READBACK 0x0A
#define CMD_SET_VOL 0x87
#define CMD_SET_RVOL 0x96
#define CMD_SET_STANDBY 0x0C
#define CMD_SET_ROUTE 0x81
#define CMD_GET_INFO 0x20
#define CMD_GET_CONTROLS 0x07
#define CMD_GET_SPEAKER 0x22

#define CMD_SET_LIGHT 0x0D
#define CMD_SET_DYNAMIC 0x83
#define CMD_SET_DYNAMIC_GAIN 0x84
#define CMD_SET_DYNAMIC_TIME 0x85
#define CMD_SET_EQ 0x80
#define CMD_SET_GATE 0x89
#define CMD_SET_GATE_TIME 0x8A
#define CMD_SET_XOVER 0x82
#define CMD_SET_DELAY 0x86
#define CMD_SET_SPEAKER 0x021

extern uint8_t cmd_buf[CMD_BUF_LEN];

int cmd_begin();
int cmd_add_byte(uint8_t data);

int cmd_load_preset(int id, int nr);
int cmd_set_speaker(int id, int channel, int nr);
int cmd_get_preset(int id);
int cmd_get_speaker(int id, int channel);
int cmd_set_vol(int id, int channel, int vol, bool on, bool inv);
int cmd_set_rvol(int id, int channel, int vol);
int cmd_set_standby(int id, bool on);

int cmd_get_vol(int id, int channel);
int cmd_set_mute(int id, int channel, bool on);
int cmd_get_mute(int id, int channel);

int cmd_set_route(int id, int out_ch, int in_ch, int vol, bool on, bool inv);
int cmd_get_route(int id, int out_ch, int in_ch);
int cmd_get_standby(int id);
int cmd_get_info(int id);
int cmd_get_controls(int id);

int cmd_set_light(int id, bool on, bool sign);

int cmd_set_eq(int id, int channel, int nr, int freq, int q, int gain, bool on);
int cmd_set_xover(int id, int channel, int nr, int freq, bool on);
int cmd_set_delay(int id, int channel, int time, bool on);

int cmd_set_gate(int id, int channel, int thrs, bool on);
int cmd_set_gate_time(int id, int channel, int time);

int cmd_set_dynamic(int id, int channel, int lim, int comp, int ratio, bool on);
int cmd_set_dyn_gain(int id, int channel, int gain);
int cmd_set_dyn_time(int id, int channel, int att, int rel);

int cmd_get_light(int id);

int cmd_get_eq(int id, int channel, int nr);
int cmd_get_xover(int id, int channel, int nr);
int cmd_get_delay(int id, int channel);

int cmd_get_gate(int id, int channel);
int cmd_get_gate_time(int id, int channel);

int cmd_get_dynamic(int id, int channel);
int cmd_get_dyn_gain(int id, int channel);
int cmd_get_dyn_time(int id, int channel);

#endif /* __CMD_H */
