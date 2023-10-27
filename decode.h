/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __DECODE_H
#define __DECODE_H

#define RX_BUF_LEN 515
#define REPLY_BUF_LEN 256

extern uint8_t rx_buf[RX_BUF_LEN];

void decode_begin(int id);
int decode_rx_data(uint8_t *raw, int len);
int get_preset(int *nr, char *name, int num);
int get_speaker(int *nr, char *name, int num);
int get_gain(int16_t *gain, bool *on, bool *inv);
int get_mute(bool *on);
int get_standby(bool *on);
int get_info(uint16_t *device_class, uint8_t *version);
int get_temp(short *temp);
int get_stat(uint8_t *stat);
int get_light(bool *on, bool *sign);
int get_eq(uint16_t *freq, uint16_t *q, int16_t *gain, bool *on);
int get_xover(int16_t *freq, bool *on);
int get_time(uint16_t *time, bool *on);
int get_gate(int16_t *thrs, bool *on);
int get_gate_time(uint16_t *time);
int get_dynamic(int16_t *lim, int16_t *comp, int16_t *ratio, bool *on);
int get_dyn_gain(int16_t *gain);
int get_dyn_time(uint16_t *att, uint16_t *rel);

#endif /* __DECODE_H */
