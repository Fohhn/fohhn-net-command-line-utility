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
#include <stdint.h>

#include "serial.h"

int init_serial_port(int *fd_ptr, char *tty, int baud)
{
  int fd;
  struct termios options;
  speed_t speed = B19200;
  memset(&options, 0, sizeof options);

  switch (baud)
  {
  case 9600:
    speed = B9600;
    break;
  case 19200:
    speed = B19200;
    break;
  case 38400:
    speed = B38400;
    break;
  case 57600:
    speed = B57600;
    break;
  case 115200:
    speed = B115200;
    break;
  default:
    return SERIAL_ERROR_BAUDRATE;
  }

  fd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1)
    return SERIAL_ERROR;

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

  cfsetispeed(&options, speed);
  cfsetospeed(&options, speed);

  usleep(20000); /* rx buf flush requires delay? */
  tcsetattr(fd, TCSAFLUSH, &options);

  *fd_ptr = fd;

  return SERIAL_OK;
}

void write_to_serial_port(int *fd_ptr, uint8_t *buf, int len)
{
  tcflush(*fd_ptr, TCIOFLUSH); /* io flush before write? */
  write(*fd_ptr, buf, len);
}

int read_from_serial_port(int *fd_ptr, uint8_t *buf, int max_len)
{
  return read(*fd_ptr, buf, max_len);
}
