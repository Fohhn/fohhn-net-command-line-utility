# Fohhn-Net command line utility

Simple Fohhn-Net command line utility for Linux.

## Table of contents
- [Fohhn-Net command line utility](#fohhn-net-command-line-utility)
  - [Table of contents](#table-of-contents)
  - [Build](#build)
  - [Man page](#man-page)
  - [License](#license)

## Build
Use 'make' to build the utility.

## Requirements
Add user to group 'dialout' to access serial ports.
```shell 
usermod -a -G dialout username
```

## Man page

NAME
       
       fnet - command line control for Fohhn DSP devices

SYNOPSIS

       fnet -t tty -d id -gc command [-p parameters]

       fnet -t tty -d id -sc command -p parameters

DESCRIPTION

       Command line utility to control Fohhn DSP devices over a serial port or
       udp node. Alle values are written without delimiter,  for  example  for
       -20.5dB write -205.

OPTIONS

       -t     serial device

       -n     udp node

       -d     id

       -g     get (default)

       -s     set

       -c     command

              preset  vol  rvol  mute  route standby info temp status light eq
              xover delay gate gatetime dyn dyngain dyntime speaker


       -p     comma-separted list of command parameters

              nr,ch,in,gain,on,off,inv,sign,freq,q,time,thrs,lim,comp,ra-
              tio,att,rel


       -v     verbose output

       -h     show help

       -V     show version info

EXAMPLES

       Load preset 3 on device id 1

              fnet -t /dev/ttyUSB0 -d 1 -sc preset -p nr=3
			  
	   Load preset 3 on device id 1 (connected over IP)

              fnet -n 192.168.1.4 -d 1 -sc preset -p nr=3

       Load speaker preset 10 on device channel 1

              fnet -t /dev/ttyUSB0 -d 1 -sc speaker -p ch=1,nr=10

       Set output volume gain to -30.0dB on channel 1

              fnet -t /dev/ttyUSB0 -d 1 -sc vol -p ch=1,gain=-300,on

       Increase output volume for +1.5dB on channel 1

              fnet -t /dev/ttyUSB0 -d 1 -sc rvol -p ch=1,gain=15

       Mute output channel 1

              fnet -t /dev/ttyUSB0 -d 1 -sc mute -p ch=1,off

       Unmute output channel 1

              fnet -t /dev/ttyUSB0 -d 1 -sc mute -p ch=1,on

       Set routing input 1 with -10.0dB to output 1

              fnet -t /dev/ttyUSB0 -d 1 -sc route -p ch=1,in=1,gain=-100,on

       Set standby for device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc standby -p on

       Set no standby for device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc standby -p off

       Get current preset number and name

              fnet -t /dev/ttyUSB0 -d 1 -gc preset

       Get current speaker number and name on channel 1

              fnet -t /dev/ttyUSB0 -d 1 -gc speaker -p ch=1

       Get gain, mute and inv for ch 1

              fnet -t /dev/ttyUSB0 -d 1 -gc vol -p ch=1

       Get device identifier and firmware version for device 1

              fnet -t /dev/ttyUSB0 -d 1 -gc info

       Get temperature of device 1

              fnet -t /dev/ttyUSB0 -d 1 -gc temp

       Get hardware status flags of device 1

              fnet -t /dev/ttyUSB0 -d 1 -gc status

       Disable blue light of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc light -p off

       Enable blue light of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc light -p on

       Sign (blink) blue light of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc light -p sign

       Set equalizer filter 1 on channel 1 to 35Hz, +2.0dB, Q 2.3 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc eq -p ch=1,nr=1,gain=20,freq=35,q=23,on

       Enable highpass on channel 1 at 200Hz of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc xover -p ch=1,nr=1,freq=200,on

       Enable lowpass on channel 1 at 16kHz of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc xover -p ch=1,nr=2,freq=16000,on

       Set 30ms delay on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc delay -p ch=1,time=3000,on

       Enable gate at -74.0dB on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc gate -p ch=1,thrs=-740,on

       Set gate time to 5 seconds on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc gatetime -p ch=1,time=5

       Set  dynamic  limiter to +20dB and compressor to +12.0dB with ratio 2.2
       on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc dyn -p ch=1,lim=200,comp=120,ratio=22,on

       Set dynamic postgain to +3.0dB on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc dyngain -p ch=1,gain=30

       Set dynamic attack to 20.0ms and release to 200.0ms on ch 1 of device 1

              fnet -t /dev/ttyUSB0 -d 1 -sc dyntime -p ch=1,att=200,rel=2000


## License
fohhn-net-command-line-utility licensed under the [MIT License](https://opensource.org/licenses/MIT), see [LICENSE](LICENSE) for more information.
