/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#define SERIAL_ERROR_BAUDRATE 2
#define SERIAL_ERROR 1
#define SERIAL_OK 0

int init_serial_port(int *fd_ptr, char *tty, int baud);
void write_to_serial_port(int *fd_ptr, unsigned char *buf, int len);
int read_from_serial_port(int *fd_ptr, unsigned char *buf, int max_len);

#endif /* __SERIAL_H */
