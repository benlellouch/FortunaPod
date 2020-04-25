#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>


#include "audio/audio.h"
#include "lcd/lcd.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define MAX_SONGS 10
#define CURSOR_UP 1
#define CURSOR_DOWN 0
#define CURSOR_VISUAL_OFFSET_Y 13
#define CURSOR_VISUAL_OFFSET_X 2
#define DUMMY 1

FILINFO songs[MAX_SONGS];
uint8_t cursor = 0;
uint8_t num_of_songs = 0;

void update_cursor(uint8_t cursor_direction)
{

	if (cursor_direction == CURSOR_UP)
	{
		if(cursor == (num_of_songs - 1))
		{
			display_string_xy_inverted(songs[cursor].fname, CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y);
			display_string_xy(songs[0].fname, CURSOR_VISUAL_OFFSET_X,CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy_inverted(songs[cursor].fname, CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y);
			display_string_xy(songs[cursor + 1].fname, CURSOR_VISUAL_OFFSET_X, ((cursor + 1) * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		

	}
	else
	{
		if (cursor == 0)
		{
			display_string_xy_inverted(songs[cursor].fname, CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y );
			display_string_xy(songs[num_of_songs - 1].fname, CURSOR_VISUAL_OFFSET_X, ((num_of_songs - 1) * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy_inverted(songs[cursor].fname, CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y );
			display_string_xy(songs[cursor - 1].fname, CURSOR_VISUAL_OFFSET_X, ((cursor - 1) * 8) + CURSOR_VISUAL_OFFSET_Y );
		}
		

	}
	
}

void generate_top_delimiter()
{
	rectangle rect;
	rect.left = 0;
	rect.right = 319;
	rect.top = 10;
	rect.bottom = 11;
	fill_rectangle(rect, WHITE);
}

void boot_screen_animation()
{
	int ftn_delay = 300;
	int pod_delay = 600;

	_delay_ms(ftn_delay);
	display_string_xy("FOR", 120 , 120);
	_delay_ms(ftn_delay);
	display_string("TU");
	_delay_ms(ftn_delay);
	display_string("NA");
	_delay_ms(pod_delay);
	display_string("-");
	_delay_ms(pod_delay);
	display_string("P");
	_delay_ms(pod_delay);
	display_string("O");
	_delay_ms(pod_delay);
	display_string("D");
	_delay_ms(pod_delay);
	display_string_xy("By Benjamin Lellouch", 110, 130);
	_delay_ms(1500);
}

void set_up_screen()
{
	clear_screen();
	boot_screen_animation();
	clear_screen();
	display_string_xy("FORTUNA-POD", 130, 1);
	generate_top_delimiter();
	for ( int i = 0; i < num_of_songs; i ++)
	{
		if (i == cursor)
		{
			display_string_xy_inverted(songs[i].fname, CURSOR_VISUAL_OFFSET_X , (i * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy(songs[i].fname, CURSOR_VISUAL_OFFSET_X, (i*8) + CURSOR_VISUAL_OFFSET_Y);
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

void generate_dummy_songs()
{
	FILINFO jul;
	strcpy(jul.fname,"JUL");
	songs[0] = jul;
	FILINFO kaaris;
	strcpy(kaaris.fname,"Kaaris");
	songs[1] = kaaris;
	FILINFO booba;
	strcpy(booba.fname,"Booba");
	songs[2] = booba;
	num_of_songs = 3;
}

int main(void) {
	/* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
	
	init_lcd();


	FATFS FatFs;
	DIR dir;
	if(DUMMY)
	{
		generate_dummy_songs();
		set_up_screen();
		os_init_scheduler();
		os_init_ruota();
		os_add_task(collect_delta, 100, 0);
		os_add_task(check_switches, 100, 0);
		sei();


	}
	else
	{
		if(f_mount(&FatFs, "",0) == FR_OK)
		{
			FRESULT res = f_opendir(&dir, "/");
			printf("%d", (int)res);
			if ( res == FR_OK)
			{
				FILINFO info;
				FRESULT res = f_readdir(&dir, &info);
				while((res == FR_OK) && (num_of_songs < MAX_SONGS) && (info.fname[0] != 0))
				{
					songs[num_of_songs] = info;
					num_of_songs ++;
					res = f_readdir(&dir, &info);
				}

				set_up_screen();
				os_init_scheduler();
				os_init_ruota();
				os_add_task(collect_delta, 100, 0);
				os_add_task(check_switches, 100, 0);
				sei();


			}
			else
			{
				display_string(" \n cannot open directory");
			}
		}
		else
		{
			display_string("Failed to mount drive");
		}
	}
	





	
	
	
	
	for(;;);
}

