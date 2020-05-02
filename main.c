#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>


#include "audio/audio.h"
#include "graphics/music_gfx.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define MAX_SONGS 10
#define CURSOR_UP 1
#define CURSOR_DOWN 0
#define CURSOR_VISUAL_OFFSET_Y 15
#define CURSOR_VISUAL_OFFSET_X 4
#define BUTTON_SCALE 0.5
#define BUTTON_Y 195
#define PAUSE_BUTTON_X 142
#define FFORWARD_BUTTON_X 210
#define FBACKWARD_BUTTON_X 110
#define DUMMY false

typedef enum 
{
	SONG_SELECT = 0,
	MUSIC_CONTROL
}   SCREEN_CONTEXT;


TCHAR songs[MAX_SONGS][13];
uint8_t cursor = 0;
uint8_t control_cursor = 0;
uint8_t num_of_songs = 0;
int current_song = -1;
int next_song = -1;
SCREEN_CONTEXT current_context = SONG_SELECT;



void update_cursor(uint8_t cursor_direction)
{

	if (cursor_direction == CURSOR_UP)
	{
		if(cursor == (num_of_songs - 1))
		{
			display_string_xy_inverted(songs[cursor], CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y);
			display_string_xy(songs[0], CURSOR_VISUAL_OFFSET_X,CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy_inverted(songs[cursor], CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y);
			display_string_xy(songs[cursor + 1], CURSOR_VISUAL_OFFSET_X, ((cursor + 1) * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		

	}
	else
	{
		if (cursor == 0)
		{
			display_string_xy_inverted(songs[cursor], CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y );
			display_string_xy(songs[num_of_songs - 1], CURSOR_VISUAL_OFFSET_X, ((num_of_songs - 1) * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy_inverted(songs[cursor], CURSOR_VISUAL_OFFSET_X, (cursor * 8) + CURSOR_VISUAL_OFFSET_Y );
			display_string_xy(songs[cursor - 1], CURSOR_VISUAL_OFFSET_X, ((cursor - 1) * 8) + CURSOR_VISUAL_OFFSET_Y );
		}
		

	}
	
}

void update_control_cursor()
{
	if(control_cursor == 0)
	{
		draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
		draw_fbackward_button_selected(FBACKWARD_BUTTON_X,BUTTON_Y ,BUTTON_SCALE);
	}
	else if (control_cursor == 1)
	{
		draw_pause_button_selected(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
		draw_fforward_button(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
		draw_fbackward_button(FBACKWARD_BUTTON_X,BUTTON_Y ,BUTTON_SCALE);
	}
	else if (control_cursor == 2)
	{
		draw_fforward_button_selected(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
		draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
	}
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
	display_string_xy("By Benjamin Lellouch", 100, 130);
	_delay_ms(1500);
}

void no_song_playing()
{
	if(current_context == SONG_SELECT)
	{
	display_string_xy("No song playing", 120, 230);
	draw_rectangle(0, LCDHEIGHT, 180, 190, BLACK);
	}
	else if (current_context == MUSIC_CONTROL)
	{
	display_string_xy("No song playing", 120 , 180);
	draw_rectangle(0, LCDHEIGHT, 230, LCDWIDTH, BLACK);
	}
}

void song_playing()
{
	if (current_context == SONG_SELECT)
	{
		draw_rectangle(0, LCDHEIGHT, 180, 190, BLACK);
		draw_rectangle(160, LCDHEIGHT, 230, LCDWIDTH, BLACK);
		char playing[] = "Playing: ";
		display_string_xy(playing, 110, 229);
		display_string_xy(songs[current_song], 160, 229);
		
	}
	else if (current_context == MUSIC_CONTROL)
	{
		draw_rectangle(0, LCDHEIGHT, 230, LCDWIDTH, BLACK);
		draw_rectangle(160, LCDHEIGHT, 180, 190, BLACK);
		char playing[] = "Playing: ";
		display_string_xy(playing, 110, 180);	
		display_string_xy(songs[current_song], 160, 180);
	}

}

int check_song_playing(int state)
{
	if(current_song == -1)
	{
		no_song_playing();
	}
	else
	{
		song_playing();
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
			f_open(&song, songs[next_song], FA_READ);
			audio_load(&song);
		}


	}

	return state;
}



void display_home_screen()
{
	clear_screen();
	display_string_xy("FORTUNA-POD", 130, 1);
	draw_horizontal_delimiter(10);
	draw_horizontal_delimiter(227);
	for ( int i = 0; i < num_of_songs; i ++)
	{
		if (i == cursor)
		{
			display_string_xy_inverted(songs[i], CURSOR_VISUAL_OFFSET_X , (i * 8) + CURSOR_VISUAL_OFFSET_Y);
		}
		else
		{
			display_string_xy(songs[i], CURSOR_VISUAL_OFFSET_X, (i*8) + CURSOR_VISUAL_OFFSET_Y);
		}
	}
	// check_song_playing(0);
}

void set_up_screen()
{
	clear_screen();
	boot_screen_animation();
	display_home_screen();

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
	rectangle rect = {0, 319, 120, 239};
	fill_rectangle(rect, BLACK);
	draw_horizontal_delimiter(175);
	draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
	draw_fforward_button(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
	draw_fbackward_button(FBACKWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
	control_cursor = 1;
	update_control_cursor();
	// check_song_playing(0);
}

void hide_music_controls()
{
	draw_rectangle(0, LCDHEIGHT, 175, LCDWIDTH, BLACK);
	draw_horizontal_delimiter(227);
	// check_song_playing(0);
}



int check_switches(int state)
{
	if (get_switch_press(_BV(SWC))) 
	{
		if(current_context == SONG_SELECT)
		{
			// current_song = cursor;
			// check_song_playing();
			// if(!DUMMY)
			// {
			// 	FIL song;
			// 	//TODO check for FR_RESULT
			// 	f_open(&song, songs[cursor],FA_READ);
			// 	audio_load(&song);
			// }

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
		} 
	}

	if(current_context == MUSIC_CONTROL)
	{
		if (get_switch_press(_BV(SWC)))
		{
			switch (control_cursor)
			{
			case 0:
				play_previous_song();
				break;
			case 1:
				pause_song();
				break;
			case 2:
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
		}

		if (get_switch_press(_BV(SWE)))
		{
			if (control_cursor < 2)
			{
				control_cursor++;
				update_control_cursor();
			}	
		}

		if(get_switch_press(_BV(SWW)))
		{
			if(control_cursor > 0)
			{
				control_cursor -= 1;
				update_control_cursor();
			}	
		}
	}

	return state;
}

void generate_dummy_songs()
{
	strcpy(songs[0], "jul");
	strcpy(songs[1], "kaaris");
	strcpy(songs[2], "booba");


	num_of_songs = 3;
}

void os_init()
{	
	set_up_screen();
	os_init_scheduler();
	os_init_ruota();
	os_add_task(check_music_controls, 100, 0);
	os_add_task(check_song_playing, 300,0);
	os_add_task(check_next_song, 100, 0);
	os_add_task(collect_delta, 100, 0);
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
					strcpy(songs[num_of_songs], info.fname);
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

