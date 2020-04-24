/*
 * https://github.com/fatcookies
 * 
 * La Fortuna (at90usb1286) @ 8MHz
 * Plays PCM audio on OC1A and OC3A up to 16Hz sample rate with 8bit samples.
 * 
 * Adapted from:
 * http://avrpcm.blogspot.co.uk/2010/11/playing-8-bit-pcm-using-any-avr.html 
 */
 
#ifndef _AUDIO_H
#define _AUDIO_H

#ifndef _FATFS
#include "ff.h"
#endif

#ifndef _STDINT_H 
#include <stdint.h>
#endif

#define SWAP_UINT16(x) ((((x) >> 8) & 0x00FF) | (((x) << 8) & 0xFF00))
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#define BUFFER_SIZE 4096

uint8_t audio_isplaying();
void audio_close();
FRESULT audio_load(FIL* audiofile);

#endif /* _AUDIO_H */
