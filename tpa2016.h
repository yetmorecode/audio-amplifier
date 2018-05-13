#ifndef __TPA2016_H
#define __TPA2016_H

#define TPA2016_ADDRESS 0x58

int	tpa2016_init();
uint8_t	tpa2016_get_gain();
int 	tpa2016_set_gain();
int	tpa2016_mute();
int	tpa2016_unmute();

#endif
