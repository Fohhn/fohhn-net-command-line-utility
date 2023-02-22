/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE.TXT for more information
 */

#ifndef __UDP_H
#define __UDP_H

#define UDP_PORT 2101

#include <arpa/inet.h>

int create_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, char *node);
void write_to_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, int size, unsigned char *buf, int len);
int receive_from_udp_node(int *sock_fd, struct sockaddr_in *remote_addr, int size, char *buf, int len);

#endif /* __UDP_H */
