/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __DECODE_H
#define __DECODE_H

#define RX_BUF_LEN 515
#define REPLY_BUF_LEN 256

extern unsigned char rx_buf[RX_BUF_LEN];

void decode_begin(int id);
int decode_rx_data(unsigned char *raw, int len);
int get_preset(int *nr, char *name, int num);
int get_speaker(int *nr, char *name, int num);
int get_gain(signed short *gain, bool *on, bool *inv);
int get_mute(bool *on);
int get_standby(bool *on);
int get_info(unsigned short *device_class, unsigned char *version);
int get_temp(short *temp);
int get_stat(unsigned char *stat);
int get_light(bool *on, bool *sign);
int get_eq(unsigned short *freq, unsigned short *q, signed short *gain, bool *on);
int get_xover(signed short *freq, bool *on);
int get_time(unsigned short *time, bool *on);
int get_gate(signed short *thrs, bool *on);
int get_gate_time(unsigned short *time);
int get_dynamic(signed short *lim, signed short *comp, signed short *ratio, bool *on);
int get_dyn_gain(signed short *gain);
int get_dyn_time(signed short *att, signed short *rel);

#endif /* __DECODE_H */
