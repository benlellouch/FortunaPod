#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>

#include "audio/audio.h"
#include "graphics/music_gfx.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define DUMMY true

TCHAR songs[MAX_SONGS][FILE_LENGTH];
uint8_t cursor = 0;
CONTROL_CURSOR control_cursor = PAUSE_BUTTON;
uint8_t num_of_songs = 0;
int8_t current_song = -1;
int8_t next_song = -1;
SCREEN_CONTEXT current_context = SONG_SELECT;

void update_cursor(CURSOR_DIRECTION cursor_direction)
{
	if (cursor_direction == CURSOR_UP)
	{
		if(cursor == (num_of_songs - 1))
		{
			draw_updated_cursor(songs[0], songs[cursor], 0, (cursor * 8));
		}
		else
		{
			draw_updated_cursor(songs[cursor + 1], songs[cursor],((cursor + 1) * 8), (cursor * 8));
		}
	}
	else
	{
		if (cursor == 0)
		{
			draw_updated_cursor(songs[num_of_songs - 1], songs[cursor], ((num_of_songs - 1) * 8),(cursor * 8) );
		}
		else
		{
			draw_updated_cursor(songs[cursor - 1], songs[cursor],((cursor - 1) * 8) , (cursor * 8));
		}
	}
}

int check_song_playing(int state)
{
	if(current_song == -1 || !audio_isplaying())
	{
		draw_no_song_playing(&current_context);
	}
	else
	{
		draw_song_playing(songs[current_song], &current_context);
	}
	return state;
}

int check_next_song(int state)
{
	if(!audio_isplaying() && (next_song > -1))
	{
		current_song = next_song;
		next_song = -1;
		if(!DUMMY)
		{
			FIL song;
			FRESULT res = f_open(&song, songs[current_song], FA_READ);
			if (res == FR_OK)
			{
				audio_load(&song);
			}
		}
	}
	return state;
}

void set_up_screen()
{
	clear_screen();
	boot_screen_animation();
	display_home_screen(&num_of_songs, songs);
}


int collect_delta(int state)
{
	if(current_context == SONG_SELECT)
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
	}
		
	return state;
}

void show_music_controls()
{
	draw_music_controls();
	control_cursor = PAUSE_BUTTON;
	update_control_cursor(&control_cursor);
}

int check_switches(int state)
{
	if(current_context == SONG_SELECT)
	{
		if (get_switch_press(_BV(SWC))) 
		{
				next_song =cursor;
		}
	}
	return state;
}

void play_previous_song()
{
	if (current_song > 0 && audio_isplaying())
	{
		next_song = current_song - 1;
		audio_close();
	}
	else
	{
		next_song = num_of_songs - 1;
		audio_close();
	}
}

void pause_song()
{
	if(audio_isplaying())
	{
		audio_close();
	}
	else
	{
		next_song = cursor;
	}
}

void play_next_song()
{
	if(current_song < (num_of_songs - 1) && audio_isplaying())
	{
		next_song = current_song + 1;
		audio_close();
	}
	else
	{
		next_song = 0;
		audio_close();
	}
}


int check_music_controls(int state)
{

	if(current_context == SONG_SELECT)
	{
		if (get_switch_press(_BV(SWS)))
		{
			current_context = MUSIC_CONTROL;
			show_music_controls();
			check_song_playing(0);
		} 
	}

	if(current_context == MUSIC_CONTROL)
	{
		if (get_switch_press(_BV(SWC)))
		{
			switch (control_cursor)
			{
			case BACKWARD_BUTTON:
				play_previous_song();
				break;
			case PAUSE_BUTTON:
				pause_song();
				break;
			case FORWARD_BUTTON:
				play_next_song();
				break;		
			default:
				break;
			}
		}

		if (get_switch_press(_BV(SWN)))
		{		
			current_context = SONG_SELECT;
			hide_music_controls();
			check_song_playing(0);
		}

		if (get_switch_press(_BV(SWE)))
		{
			if (control_cursor < 2)
			{
				control_cursor++;
				update_control_cursor(&control_cursor);
			}	
		}

		if(get_switch_press(_BV(SWW)))
		{
			if(control_cursor > 0)
			{
				control_cursor -= 1;
				update_control_cursor(&control_cursor);
			}	
		}
	}

	return state;
}

void generate_dummy_songs()
{
	strcpy(songs[0], "TOTD");
	strcpy(songs[1], "Rammstein");
	strcpy(songs[2], "music.wav");
	strcpy(songs[3], "Over.wav");
	num_of_songs = 4;
}

void os_init()
{	
	set_up_screen();
	os_init_scheduler();
	os_init_ruota();
	os_add_task(check_music_controls, 100, 0);
	os_add_task(check_song_playing, 1000,0);
	os_add_task(collect_delta, 100, 0);
	os_add_task(check_next_song, 100, 0);
	os_add_task(check_switches, 100, 0);
	sei();
}

int main(void) {

	/* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

	init_lcd();

	if(DUMMY)
	{
		generate_dummy_songs();
		os_init();
	}
	else
	{		
		FATFS FatFs;
		DIR dir;
		if(f_mount(&FatFs, "",0) == FR_OK)
		{
			 FRESULT res = f_opendir(&dir, "/");
			while(res != FR_OK)
			{
				res = f_opendir(&dir, "/");
			}
			if ( res == FR_OK)
			{
				FILINFO info;
				FRESULT res = f_readdir(&dir, &info);
				while((res == FR_OK) && (num_of_songs < MAX_SONGS) && (info.fname[0] != 0))
				{
					strncpy(songs[num_of_songs],info.fname, 13);
					num_of_songs ++;
					res = f_readdir(&dir, &info);
				}

				os_init();


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

