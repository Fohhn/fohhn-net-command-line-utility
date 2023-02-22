/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "serial.h"

int init_serial_port(int *fd_ptr, char *tty)
{
  int fd;
  struct termios options;
  memset(&options, 0, sizeof options);

  fd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1)
    return 1;

  fcntl(fd, F_SETFL, 0);

  tcgetattr(fd, &options);

  cfmakeraw(&options);

  options.c_cflag = 0;
  options.c_oflag = 0;
  options.c_iflag = 0;
  options.c_lflag = 0;

  options.c_iflag = IGNBRK | IGNPAR;
  options.c_cflag = (CLOCAL | CREAD | CS8);

  options.c_cc[VTIME] = 5;
  options.c_cc[VMIN] = 0;

  cfsetispeed(&options, B19200);
  cfsetospeed(&options, B19200);

  usleep(20000); /* rx buffer flush funktioniert nicht ohne delay? */
  tcsetattr(fd, TCSAFLUSH, &options);

  *fd_ptr = fd;

  return 0;
}

void write_to_serial_port(int *fd_ptr, unsigned char *buf, int len)
{
  tcflush(*fd_ptr, TCIOFLUSH); /* io flush vor jedem write? */
  write(*fd_ptr, buf, len);
}

int read_from_serial_port(int *fd_ptr, unsigned char *buf, int max_len)
{
  return read(*fd_ptr, buf, max_len);
}
