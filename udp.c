/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <stdint.h>

#include "udp.h"

int create_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, char *node)
{
  int s, rc;
  struct hostent *h;
  struct sockaddr_in local_addr;

  h = gethostbyname(node);
  if (h == NULL)
    return -1;

  remote_addr->sin_family = h->h_addrtype;
  memcpy((char *)&remote_addr->sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  remote_addr->sin_port = htons(UDP_PORT);
  s = socket(AF_INET, SOCK_DGRAM, 0);
  if (s < 0)
    return -1;

  local_addr.sin_family = AF_INET;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  local_addr.sin_port = htons(0);
  rc = bind(s, (struct sockaddr *)&local_addr, sizeof(local_addr));
  if (rc < 0)
    return -1;

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 30000;
  rc = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
  if (rc < 0)
    return -1;

  *sock_fd = s;
  return 0;
}

void write_to_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, int size, uint8_t *buf, int len)
{
  sendto(*sock_fd, buf, len, 0, (struct sockaddr *)remote_addr, size);
}

int receive_from_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, int size, char *buf, int len)
{
  return recvfrom(*sock_fd, buf, len, 0, (struct sockaddr *)remote_addr, &size);
}
