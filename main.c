#include "audio/audio.h"
#include <avr/interrupt.h>
#include "lcd/lcd.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define MAX_SONGS 10

FILINFO songs[MAX_SONGS];
uint8_t cursor = 0;
uint8_t num_of_songs = 0;

void update_screen()
{
	clear_screen();
	int i;
	for ( i = 0; i < num_of_songs; i ++)
	{
		if (i == cursor)
		{
			display_string_inverted(songs[i].fname);
			display_string("\n");
		}
		else
		{
			display_string(songs[i].fname);
			display_string("\n");
		}
		
		
	}
}

int collect_delta(int state)
{
	int delta = os_enc_delta();
	 if (delta > 0)
	{
		cursor = (cursor + 1) % num_of_songs;
		update_screen();
	}
	else if (delta < 0)
	{
		cursor = (cursor - 1) % num_of_songs;
		update_screen();
	}
	
	return state;
}

int main(void) {
	/* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
	
	init_lcd();


	FATFS FatFs;
	DIR dir;
	f_mount(&FatFs, "",0);
	if (f_opendir(&dir, "/") == FR_OK)
	{
		FILINFO info;
		while((f_readdir(&dir, &info) == FR_OK) && (num_of_songs < MAX_SONGS))
		{
			songs[num_of_songs] = info;
			num_of_songs ++;
		}

		update_screen();
		os_init_scheduler();
		os_init_ruota();
		os_add_task(collect_delta, 100, 0);
		sei();


	}
	else
	{
		display_string("cannot open directory");
	}



	
	
	
	
	for(;;);
}

