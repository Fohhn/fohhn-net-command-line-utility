/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE.TXT for more information
 */

#ifndef __MAIN_H
#define __MAIN_H

int exec_cmd(int id, int len, int reply_len);
int exec_cmd_v(int id, int len, int reply_len_min, int reply_len_max);

#endif /* __MAIN_H */
