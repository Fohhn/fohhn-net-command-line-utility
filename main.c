/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "options.h"
#include "cmd.h"
#include "serial.h"
#include "decode.h"
#include "util.h"
#include "udp.h"

const char *version_info = "0.7";

int tty_fd;
char *udp_node;

int main(int argc, char **argv)
{
  int opt;
  unsigned char device_id = 0;
  int fncmd_mode = FNCMD_MODE_INVALID;
  int fncmd = FNCMD_INVALID;
  bool set_mode = false;
  bool get_mode = false;
  bool verbose = false;
  char *tty = NULL;
  int len = 0;
  udp_node = NULL;

  param_nr = 0;
  param_chan = 0;
  param_gain = -800;
  param_on = false;
  param_off = false;
  param_in = 0;
  param_inv = false;
  param_sign = false;
  param_freq = 0;
  param_time = 0;
  param_thrs = -800;
  param_lim = 0;
  param_comp = 0;
  param_ratio = 2.0;
  param_att = 200;
  param_rel = 2000;

  if (argc < 2)
  {
    usage(argv[0]);
    return 1;
  }

  while ((opt = getopt(argc, argv, "t:n:d:sgc:p:vhV")) != -1)
    switch (opt)
    {
    case 't':
      tty = optarg;
      break;
    case 'n':
      udp_node = optarg;
      break;
    case 'd':
      device_id = atoi(optarg);
      break;
    case 's':
      set_mode = true;
      break;
    case 'g':
      get_mode = true;
      break;
    case 'c':
      fncmd = getfnetcmd(optarg);
      if (fncmd == FNCMD_INVALID)
      {
        printf("%s: invalid command\n", argv[0]);
        return 1;
      }
      break;
    case 'p':
      if (readfnetparam(optarg) != 0)
      {
        printf("%s: invalid parameter\n", argv[0]);
        return 1;
      }
      break;
    case 'v':
      verbose = true;
      break;
    case 'h':
      usage(argv[0]);
      return 0;
      break;
    case 'V':
      printf("%s %s\n", argv[0], version_info);
      return 0;
      break;
    default:
      printf("try `%s -h' for more information\n", argv[0]);
      return 1;
    }

  if (set_mode == false && get_mode == true)
  {
    fncmd_mode = FNCMD_MODE_GET;
  }
  else if (set_mode == true && get_mode == false)
  {
    fncmd_mode = FNCMD_MODE_SET;
  }
  else if (set_mode == false && get_mode == false)
  {
    fncmd_mode = FNCMD_MODE_GET;
  }
  else
  {
    printf("invalid mode\n");
    return 1;
  }

  if (device_id < 1 || device_id > 254)
  {
    printf("invalid device id\n");
    return 1;
  }

  if (param_on && param_off)
  {
    printf("invalid parameters\n");
    return 1;
  }

  if (tty != NULL && udp_node != NULL)
  {
    printf("either udp node or serial device\n");
    return 1;
  }

  if (tty != NULL)
  {
    if (init_serial_port(&tty_fd, tty) != 0)
    {
      printf("can't open %s\n", tty);
      return 1;
    }
  }
  else if (udp_node == NULL)
  {
    printf("serial device or udp node required\n");
    return 1;
  }

  if (fncmd_mode == FNCMD_MODE_SET)
  {

    switch (fncmd)
    {
    case FNCMD_PRESET:
      if (param_nr >= 1 && param_nr <= 100)
      {
        len = cmd_load_preset(device_id, param_nr);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid preset number `%d'\n", param_nr);
        return 1;
      }
      break;
    case FNCMD_SPEAKER:
      if (param_chan >= 1 && param_chan <= 255 && param_nr >= 1 && param_nr <= 100)
      {
        len = cmd_set_speaker(device_id, param_chan, param_nr);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid parameter\n");
        return 1;
      }
      break;
    case FNCMD_VOL:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_vol(device_id, param_chan, param_gain, param_on, param_inv);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_nr);
        return 1;
      }
      break;
    case FNCMD_RVOL:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_rvol(device_id, param_chan, param_gain);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_nr);
        return 1;
      }
      break;
    case FNCMD_MUTE:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_mute(device_id, param_chan, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_nr);
        return 1;
      }
      break;
    case FNCMD_ROUTE:
      if (param_chan >= 1 && param_chan <= 255 && param_in >= 1 && param_in <= 255)
      {
        len = cmd_set_route(device_id, param_chan, param_in, param_gain, param_on, param_inv);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_STANDBY:
      len = cmd_set_standby(device_id, param_on);
      if (exec_cmd(device_id, len, 0) == 0)
      {
        printf("OK\n");
      }
      break;
    case FNCMD_LIGHT:
      len = cmd_set_light(device_id, param_on, param_sign);
      if (exec_cmd(device_id, len, 0) == 0)
      {
        printf("OK\n");
      }
      break;
    case FNCMD_EQ:
      if ((param_chan >= 1 && param_chan <= 255) && (param_nr >= 1 && param_nr <= 10) && (param_freq >= 10 && param_freq <= 20000) && (param_q >= 1 && param_q <= 1000) && (param_gain >= -120 && param_gain <= 120))
      {
        len = cmd_set_eq(device_id, param_chan, param_nr, param_freq, param_q, param_gain, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid parameter\n");
        return 1;
      }
      break;
    case FNCMD_XOVER:
      if ((param_chan >= 1 && param_chan <= 255) && (param_nr >= 1 && param_nr <= 2) && (param_freq >= 10 && param_freq <= 20000))
      {
        len = cmd_set_xover(device_id, param_chan, param_nr, param_freq, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid parameter\n");
        return 1;
      }
      break;
    case FNCMD_DELAY:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_delay(device_id, param_chan, param_time, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_GATE:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_gate(device_id, param_chan, param_thrs, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_GATETIME:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_gate_time(device_id, param_chan, param_time);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_DYN:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_dynamic(device_id, param_chan, param_lim, param_comp, param_ratio, param_on);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_DYNGAIN:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_dyn_gain(device_id, param_chan, param_gain);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_DYNTIME:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_set_dyn_time(device_id, param_chan, param_att, param_rel);
        if (exec_cmd(device_id, len, 0) == 0)
        {
          printf("OK\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;

    default:
      printf("%s: invalid arguments\ntry `%s -h' for more information\n", argv[0], argv[0]);
      return 1;
      break;
    }
  }
  else if (fncmd_mode == FNCMD_MODE_GET)
  {
    switch (fncmd)
    {
    case FNCMD_PRESET:
      len = cmd_get_preset(device_id);
      if (exec_cmd(device_id, len, 20) == 0)
      {
        int nr;
        char name[16];
        get_preset(&nr, name, sizeof(name));
        printf("%03d,%s\n", nr, name);
      }
      break;
    case FNCMD_SPEAKER:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_speaker(device_id, param_chan);
        if (exec_cmd(device_id, len, 20) == 0)
        {
          int nr;
          char name[16];
          get_speaker(&nr, name, sizeof(name));
          printf("%03d,%s\n", nr, name);
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_VOL:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_vol(device_id, param_chan);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          signed short gain;
          bool on, inv;
          get_gain(&gain, &on, &inv);
          printf("%d,%d,%d\n", gain, on, inv);
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_MUTE:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_vol(device_id, param_chan);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          bool on;
          get_mute(&on);
          if (on)
            printf("ON\n");
          else
            printf("OFF\n");
        }
      }
      else
      {
        printf("invalid channel `%d'\n", param_chan);
        return 1;
      }
      break;
    case FNCMD_ROUTE:
      if (param_chan >= 1 && param_chan <= 255 && param_in >= 1 && param_in <= 255)
      {
        len = cmd_get_route(device_id, param_chan, param_in);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          signed short gain;
          bool on, inv;
          get_gain(&gain, &on, &inv);
          printf("%d,%d,%d\n", gain, on, inv);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_STANDBY:
      len = cmd_get_standby(device_id);
      if (exec_cmd(device_id, len, 1) == 0)
      {
        bool on;
        get_standby(&on);
        if (on)
          printf("ON\n");
        else
          printf("OFF\n");
      }
      break;
    case FNCMD_INFO:
      len = cmd_get_info(device_id);
      if (exec_cmd(device_id, len, 5) == 0)
      {
        unsigned short device_class;
        unsigned char version[3];
        get_info(&device_class, version);
        printf("%#06x,%d.%d.%d\n", device_class, version[0], version[1], version[2]);
      }
      break;
    case FNCMD_TEMP:
      len = cmd_get_controls(device_id);
      if (exec_cmd(device_id, len, 4) == 0)
      {
        short temp;
        get_temp(&temp);
        printf("%d\n", temp);
      }
      break;
    case FNCMD_STAT:
      len = cmd_get_controls(device_id);
      if (exec_cmd_v(device_id, len, 4, 5) == 0)
      {
        unsigned char stat;
        get_stat(&stat);
        for (int i = 0; i < 8; i++)
        {
          if (BT(stat, i))
            printf("1");
          else
            printf("0");
        }
        printf("\n");
      }
      break;
    case FNCMD_LIGHT:
      len = cmd_get_light(device_id);
      if (exec_cmd(device_id, len, 1) == 0)
      {
        bool on, sign;
        get_light(&on, &sign);
        printf("%d,%d\n", on, sign);
      }
      break;
    case FNCMD_EQ:
      if (param_chan >= 1 && param_chan <= 255 && param_nr >= 1 && param_nr <= 10)
      {
        len = cmd_get_eq(device_id, param_chan, param_nr);
        if (exec_cmd(device_id, len, 7) == 0)
        {
          signed short freq, q, gain;
          bool on;
          get_eq(&freq, &q, &gain, &on);
          printf("%d,%d,%d,%d\n", freq, q, gain, on);
        }
      }
      else
      {
        printf("invalid channel or number\n");
        return 1;
      }
      break;
    case FNCMD_XOVER:
      if (param_chan >= 1 && param_chan <= 255 && param_nr >= 1 && param_nr <= 2)
      {
        len = cmd_get_xover(device_id, param_chan, param_nr);
        if (exec_cmd(device_id, len, 4) == 0)
        {
          signed short freq;
          bool on;
          get_xover(&freq, &on);
          printf("%d,%d\n", freq, on);
        }
      }
      else
      {
        printf("invalid channel or number\n");
        return 1;
      }
      break;
    case FNCMD_DELAY:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_delay(device_id, param_chan);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          unsigned short time;
          bool on;
          get_time(&time, &on);
          printf("%d,%d\n", time, on);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_GATE:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_gate(device_id, param_chan);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          signed short thrs;
          bool on;
          get_gate(&thrs, &on);
          printf("%d,%d\n", thrs, on);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_GATETIME:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_gate_time(device_id, param_chan);
        if (exec_cmd(device_id, len, 4) == 0)
        {
          unsigned short time;
          get_gate_time(&time);
          printf("%d\n", time);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_DYN:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_dynamic(device_id, param_chan);
        if (exec_cmd(device_id, len, 7) == 0)
        {
          signed short lim, comp, ratio;
          bool on;
          get_dynamic(&lim, &comp, &ratio, &on);
          printf("%d,%d,%d,%d\n", lim, comp, ratio, on);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_DYNGAIN:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_dyn_gain(device_id, param_chan);
        if (exec_cmd(device_id, len, 3) == 0)
        {
          signed short gain;
          get_dyn_gain(&gain);
          printf("%d\n", gain);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    case FNCMD_DYNTIME:
      if (param_chan >= 1 && param_chan <= 255)
      {
        len = cmd_get_dyn_time(device_id, param_chan);
        if (exec_cmd(device_id, len, 4) == 0)
        {
          unsigned short att, rel;
          get_dyn_time(&att, &rel);
          printf("%d,%d\n", att, rel);
        }
      }
      else
      {
        printf("invalid channel\n");
        return 1;
      }
      break;
    default:
      printf("%s: invalid arguments\ntry `%s -h' for more information\n", argv[0], argv[0]);
      return 1;
      break;
    }
  }

  close(tty_fd);
  return 0;
}

int exec_cmd(int id, int len, int reply_len)
{
  return exec_cmd_v(id, len, reply_len, reply_len);
}

int exec_cmd_v(int id, int len, int reply_len_min, int reply_len_max)
{
  int num, count, total;
  int sock_fd;
  struct sockaddr_in remote_addr;
  int rc, size, reply_len;
  size = sizeof(remote_addr);

  int retry;
  for (retry = 0; retry < 3; retry++)
  {
    count = 0;
    total = 0;

    if (udp_node == NULL)
    {
      write_to_serial_port(&tty_fd, cmd_buf, len);
    }
    else
    {
      rc = create_udp_node(&sock_fd, &remote_addr, udp_node);
      if (rc == 0)
        write_to_udp_node(&sock_fd, &remote_addr, size, cmd_buf, len);
    }

    decode_begin(id);
    num = -1;
    while (count < 10 && num != 0)
    {
      if (udp_node == NULL)
      {
        num = read_from_serial_port(&tty_fd, rx_buf, RX_BUF_LEN);
      }
      else
      {
        if (rc == 0)
          num = receive_from_udp_node(&sock_fd, &remote_addr, size, rx_buf, RX_BUF_LEN);
        else
          num = -1;
      }

      if (num >= 0)
      {
        total += num;
        reply_len = decode_rx_data(rx_buf, num);
        if (reply_len >= reply_len_min && reply_len <= reply_len_max)
          return 0;
      }
      count++;
    }
  }
  if (total > 0)
    printf("INVALID DATA\n");
  else
    printf("TIMEOUT\n");
  return 1;
}
