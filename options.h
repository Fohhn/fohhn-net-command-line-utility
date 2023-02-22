/*
 * Copyright 2023 Fohhn
 *
 * licensed under the MIT License, see LICENSE for more information
 */

#ifndef __OPTIONS_H
#define __OPTIONS_H

enum
{
  FNCMD_INVALID = -1,
  FNCMD_PRESET,
  FNCMD_VOL,
  FNCMD_RVOL,
  FNCMD_MUTE,
  FNCMD_ROUTE,
  FNCMD_STANDBY,
  FNCMD_INFO,
  FNCMD_TEMP,
  FNCMD_STAT,
  FNCMD_LIGHT,
  FNCMD_EQ,
  FNCMD_XOVER,
  FNCMD_DELAY,
  FNCMD_GATE,
  FNCMD_GATETIME,
  FNCMD_DYN,
  FNCMD_DYNGAIN,
  FNCMD_DYNTIME,
  FNCMD_SPEAKER
};

enum
{
  FNCMD_MODE_INVALID = -1,
  FNCMD_MODE_GET,
  FNCMD_MODE_SET
};

enum
{
  FNPARAM_NR,
  FNPARAM_CHAN,
  FNPARAM_GAIN,
  FNPARAM_IN,
  FNPARAM_ON,
  FNPARAM_OFF,
  FNPARAM_INV,
  FNPARAM_SIGN,
  FNPARAM_FREQ,
  FNPARAM_Q,
  FNPARAM_TIME,
  FNPARAM_THRS,
  FNPARAM_LIM,
  FNPARAM_COMP,
  FNPARAM_RATIO,
  FNPARAM_ATT,
  FNPARAM_REL
};

extern int param_nr;
extern int param_chan;
extern int param_gain;
extern bool param_on;
extern bool param_off;
extern int param_in;
extern bool param_inv;
extern bool param_sign;
extern int param_freq;
extern int param_q;
extern int param_time;
extern int param_thrs;
extern int param_lim;
extern int param_comp;
extern int param_ratio;
extern int param_att;
extern int param_rel;

extern const char *cmd_opts[];
extern const char *parm_opts[];

int getfnetcmd(char *subopts);
int readfnetparam(char *subopts);

void usage(char *argv0);

#endif /* __OPTIONS_H */
