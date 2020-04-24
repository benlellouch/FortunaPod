#include "audio/audio.h"
#include <avr/interrupt.h>
#include "lcd/lcd.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define MAX_SONGS 10
#define CURSOR_UP 1
#define CURSOR_DOWN 0

FILINFO songs[MAX_SONGS];
uint8_t cursor = 0;
uint8_t num_of_songs = 0;

void update_cursor(uint8_t cursor_direction)
{

	if (cursor_direction == CURSOR_UP)
	{
		if(cursor == (num_of_songs - 1))
		{
			display_string_xy_inverted(songs[cursor].fname, 0, (cursor * 8));
			display_string_xy(songs[0].fname, 0, ((0) * 8));
		}
		else
		{
			display_string_xy_inverted(songs[cursor].fname, 0, (cursor * 8));
			display_string_xy(songs[cursor + 1].fname, 0, ((cursor + 1) * 8));
		}
		

	}
	else
	{
		if (cursor == 0)
		{
			display_string_xy_inverted(songs[cursor].fname, 0, (cursor * 8));
			display_string_xy(songs[num_of_songs - 1].fname, 0, ((num_of_songs - 1) * 8));
		}
		else
		{
			display_string_xy_inverted(songs[cursor].fname, 0, (cursor * 8));
			display_string_xy(songs[cursor - 1].fname, 0, ((cursor - 1) * 8));
		}
		

	}
	
}

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
		update_cursor(CURSOR_DOWN);
	}
	else if (delta < 0)
	{
		if(cursor == 0)
		{
			cursor = (num_of_songs - 1);
		}
		else
		{
			cursor = (cursor - 1) % num_of_songs;
		}
		
		update_cursor(CURSOR_UP);
	}
	
	return state;
}

int check_switches(int state)
{
	if (get_switch_press(_BV(SWC))) 
	{
		FIL song;
		f_open(&song, songs[cursor].fname,FA_READ);
		audio_load(&song);
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
		FRESULT res = f_readdir(&dir, &info);
		while((res == FR_OK) && (num_of_songs < MAX_SONGS) && (info.fname[0] != 0))
		{
			songs[num_of_songs] = info;
			num_of_songs ++;
			res = f_readdir(&dir, &info);
		}

		update_screen();
		os_init_scheduler();
		os_init_ruota();
		os_add_task(collect_delta, 100, 0);
		os_add_task(check_switches, 100, 0);
		sei();


	}
	else
	{
		display_string("cannot open directory");
	}



	
	
	
	
	for(;;);
}

