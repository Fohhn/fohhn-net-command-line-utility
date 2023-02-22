/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "options.h"

int param_nr;
int param_chan;
int param_gain;
bool param_on;
bool param_off;
int param_in;
bool param_inv;
bool param_sign;
int param_freq;
int param_q;
int param_time;
int param_thrs;
int param_lim;
int param_comp;
int param_ratio;
int param_att;
int param_rel;

const char *cmd_opts[] =
    {
        [FNCMD_PRESET] = "preset",
        [FNCMD_VOL] = "vol",
        [FNCMD_RVOL] = "rvol",
        [FNCMD_MUTE] = "mute",
        [FNCMD_ROUTE] = "route",
        [FNCMD_STANDBY] = "standby",
        [FNCMD_INFO] = "info",
        [FNCMD_TEMP] = "temp",
        [FNCMD_STAT] = "status",
        [FNCMD_LIGHT] = "light",
        [FNCMD_EQ] = "eq",
        [FNCMD_XOVER] = "xover",
        [FNCMD_DELAY] = "delay",
        [FNCMD_GATE] = "gate",
        [FNCMD_GATETIME] = "gatetime",
        [FNCMD_DYN] = "dyn",
        [FNCMD_DYNGAIN] = "dyngain",
        [FNCMD_DYNTIME] = "dyntime",
        [FNCMD_SPEAKER] = "speaker",
        NULL};

const char *param_opts[] =
    {
        [FNPARAM_NR] = "nr",
        [FNPARAM_CHAN] = "ch",
        [FNPARAM_GAIN] = "gain",
        [FNPARAM_IN] = "in",
        [FNPARAM_ON] = "on",
        [FNPARAM_OFF] = "off",
        [FNPARAM_INV] = "inv",
        [FNPARAM_SIGN] = "sign",
        [FNPARAM_FREQ] = "freq",
        [FNPARAM_Q] = "q",
        [FNPARAM_TIME] = "time",
        [FNPARAM_THRS] = "thrs",
        [FNPARAM_LIM] = "lim",
        [FNPARAM_COMP] = "comp",
        [FNPARAM_RATIO] = "ratio",
        [FNPARAM_ATT] = "att",
        [FNPARAM_REL] = "rel",
        NULL};

void usage(char *argv0)
{
  printf("Usage:\n");
  printf(" %s -t <tty> -d <id> -gc <command> [-p <parameters>]\n", argv0);
  printf(" %s -t <tty> -d <id> -sc <command> -p <parameters>\n", argv0);
  printf("Options:\n");
  printf(" -t serial device\n");
  printf(" -n udp node\n");
  printf(" -d id\n");
  printf(" -g get (default)\n");
  printf(" -s set\n");
  printf(" -c command\n");
  printf("    preset,vol,rvol,mute,route,standby,info,temp,status,light,eq,xover,delay,\n");
  printf("    gate,gatetime,dyn,dyngain,dyntime,speaker\n");
  printf(" -p comma-separted list of command parameters\n");
  printf("    nr,ch,in,gain,on,off,inv,sign,freq,q,time,thrs,lim,comp,ratio,att,rel\n");
  printf(" -v verbose output\n");
  printf(" -h show help\n");
  printf(" -V show version info\n");
}

int getfnetcmd(char *subopts)
{
  char *value;
  int suboptcount = 0;
  int result;
  while (*subopts != '\0')
  {
    suboptcount++;
    result = getsubopt(&subopts, (char **)cmd_opts, &value);
  }
  if (suboptcount != 1)
    result = -1;
  return result;
}

int readfnetparam(char *subopts)
{
  char *value;
  while (*subopts != '\0')
  {
    switch (getsubopt(&subopts, (char **)param_opts, &value))
    {
    case FNPARAM_NR:
      if (value == NULL)
        return 1;
      param_nr = atoi(value);
      break;
    case FNPARAM_CHAN:
      if (value == NULL)
        return 1;
      param_chan = atoi(value);
      break;
    case FNPARAM_GAIN:
      if (value == NULL)
        return 1;
      param_gain = atoi(value);
      break;
    case FNPARAM_IN:
      if (value == NULL)
        return 1;
      param_in = atoi(value);
      break;
    case FNPARAM_ON:
      param_on = true;
      break;
    case FNPARAM_OFF:
      param_off = true;
      break;
    case FNPARAM_INV:
      param_inv = true;
      break;
    case FNPARAM_SIGN:
      param_sign = true;
      break;
    case FNPARAM_FREQ:
      param_freq = atoi(value);
      break;
    case FNPARAM_Q:
      param_q = atoi(value);
      break;
    case FNPARAM_TIME:
      param_time = atoi(value);
      break;
    case FNPARAM_THRS:
      param_thrs = atoi(value);
      break;
    case FNPARAM_LIM:
      param_lim = atoi(value);
      break;
    case FNPARAM_COMP:
      param_comp = atoi(value);
      break;
    case FNPARAM_RATIO:
      param_ratio = atoi(value);
      break;
    case FNPARAM_ATT:
      param_att = atoi(value);
      break;
    case FNPARAM_REL:
      param_rel = atoi(value);
      break;
    default:
      return 1;
      break;
    }
  }

  return 0;
}
