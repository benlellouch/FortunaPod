#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdbool.h>


#include "audio/audio.h"
// #include "lcd/lcd.h"
#include "lcd/bresenham.h"
#include "ruota/ruota.h"
#include "rios/rios.h"

#define MAX_SONGS 10
#define CURSOR_UP 1
#define CURSOR_DOWN 0
#define CURSOR_VISUAL_OFFSET_Y 15
#define CURSOR_VISUAL_OFFSET_X 4
#define DUMMY false

typedef enum 
{
	SONG_SELECT = 0,
	MUSIC_CONTROL
}   SCREEN_CONTEXT;


FILINFO songs[MAX_SONGS];
uint8_t cursor = 0;
uint8_t control_cursor = 0;
uint8_t num_of_songs = 0;
char* current_song = "";
SCREEN_CONTEXT current_context = SONG_SELECT;



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

void update_control_cursor()
{
	if(control_cursor == 0)
	{
		draw_pause_button(120, 160);
		draw_fbackward_button_selected(95,160);
	}
	else if (control_cursor == 1)
	{
		draw_pause_button_selected(120, 160);
		draw_fforward_button(220, 160);
		draw_fbackward_button(95,160);
	}
	else if (control_cursor == 2)
	{
		draw_fforward_button_selected(220, 160);
		draw_pause_button(120, 160);
	}
}


void draw_horizontal_delimiter(int y)
{
	rectangle rect = {0, 319, y, y + 1};
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
	display_string_xy("By Benjamin Lellouch", 100, 130);
	_delay_ms(1500);
}

void no_song_playing()
{
	if(current_context == SONG_SELECT)
	{
	display_string_xy("No song playing", 120, 230);
	}
	else if (current_context == MUSIC_CONTROL)
	{
	display_string_xy("No song playing", 120 , 122);
	}
}

void song_playing(char* song)
{
	if (current_context == SONG_SELECT)
	{
		rectangle rect = {0, 319, 229, LCDWIDTH};
		char playing[] = "Playing: ";
		strcat(playing, song);
		strcat(playing, "     ");
		fill_rectangle(rect, BLACK);
		display_string_xy(playing, 110, 229);
	}
	else if (current_context == MUSIC_CONTROL)
	{
		rectangle rect = {0, 319, 122, 131};
		char playing[] = "Playing: ";
		strcat(playing, song);
		strcat(playing, "     ");
		fill_rectangle(rect, BLACK);
		display_string_xy(playing, 110, 122);		
	}

}

void check_song_playing()
{
	if(strcmp(current_song, "") == 0)
	{
		no_song_playing();
	}
	else
	{
		song_playing(current_song);
	}
}

void display_home_screen()
{
	clear_screen();
	display_string_xy("FORTUNA-POD", 130, 1);
	draw_horizontal_delimiter(10);
	draw_horizontal_delimiter(227);
	check_song_playing();
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
	draw_horizontal_delimiter(120);
	check_song_playing();
	draw_pause_button(120, 160);
	draw_fforward_button(220, 160);
	draw_fbackward_button(95, 160);
	control_cursor = 1;
	update_control_cursor();
}

void draw_pause_button(int x, int y)
{
	// rectangle outer_rec = {x ,x + 66, y , y + 66 };
	// rectangle inner_rec = { x + 3 , x + 63 , y + 3 , y + 63 };
	// fill_rectangle(outer_rec, WHITE);
	// fill_rectangle(inner_rec, BLACK);
	rectangle outer_rec1 = {x, x + 24,  y , y + 64};
	rectangle inner_rec1 = {x + 2, x + 22, y + 2, y + 62};
	rectangle outer_rec2 = {x + 48,  x + 72, y, y + 64};
	rectangle inner_rec2 = {x + 50, x + 70, y + 2 , y + 62};
	fill_rectangle(outer_rec1, WHITE);
	fill_rectangle(outer_rec2, WHITE);
	fill_rectangle(inner_rec1, BLACK);
	fill_rectangle(inner_rec2, BLACK);
}

void draw_pause_button_selected(int x, int y)
{
	rectangle outer_rec1 = {x, x + 24,  y , y + 64};
	rectangle outer_rec2 = {x + 48,  x + 72, y, y + 64};
	fill_rectangle(outer_rec1, WHITE);
	fill_rectangle(outer_rec2, WHITE);	
}

void draw_fforward_button(int x, int y)
{
	drawLine(x, y , x , y + 64, WHITE);
	drawLine(x , y, x + 32, y + 32, WHITE);
	drawLine(x , y + 64, x +32, y +32, WHITE);
	int fill_y = y + 1;
	int fill_x = x;
	int length = 62;
	while(fill_x < (x + 32))
	{
		fill_x += 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, BLACK); 
	}
}

void draw_fforward_button_selected(int x, int y)
{
	drawLine(x, y , x , y + 64, WHITE);
	drawLine(x , y, x + 32, y + 32, WHITE);
	drawLine(x , y + 64, x +32, y +32, WHITE);
	int fill_y = y;
	int fill_x = x;
	int length = 64;
	while(fill_x < (x + 32))
	{
		fill_x += 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, WHITE); 
	}
}

void draw_fbackward_button(int x, int y)
{
	drawLine(x, y , x , y + 64, WHITE);
	drawLine(x , y, x - 32, y + 32, WHITE);
	drawLine(x , y + 64, x - 32, y +32, WHITE);
	int fill_y = y + 1;
	int fill_x = x;
	int length = 62;
	while(fill_x > (x - 32))
	{
		fill_x -= 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, BLACK); 
	}
}

void draw_fbackward_button_selected(int x, int y)
{
	drawLine(x, y , x , y + 64, WHITE);
	drawLine(x , y, x - 32, y + 32, WHITE);
	drawLine(x , y + 64, x - 32, y +32, WHITE);
	int fill_y = y;
	int fill_x = x;
	int length = 64;
	while(fill_x > (x - 32))
	{
		fill_x -= 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, WHITE); 
	}
}

int check_switches(int state)
{
	if (get_switch_press(_BV(SWC))) 
	{
		if(current_context == SONG_SELECT)
		{
			if(!DUMMY)
			{
				FIL song;
				f_open(&song, songs[cursor].fname,FA_READ);
				audio_load(&song);
			}
			current_song = songs[cursor].fname;
			song_playing(songs[cursor].fname);
		}
	}

	if (get_switch_press(_BV(SWS)))
	{
		if(current_context == SONG_SELECT)
		{
			current_context = MUSIC_CONTROL;
			show_music_controls();
		}
	} 

	if (get_switch_press(_BV(SWN)))
	{
		if(current_context == MUSIC_CONTROL)
		{
			current_context = SONG_SELECT;
			display_home_screen();
		}
	}

	if (get_switch_press(_BV(SWE)))
	{
		if (current_context == MUSIC_CONTROL)
		{
			if (control_cursor < 2)
			{
				control_cursor++;
				update_control_cursor();
			}
		}
		
	}

	if(get_switch_press(_BV(SWW)))
	{
		if (current_context == MUSIC_CONTROL)
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
	FILINFO jul;
	strcpy(jul.fname,"JUL");
	songs[0] = jul;
	FILINFO kaaris;
	strcpy(kaaris.fname,"Kaari");
	songs[1] = kaaris;
	FILINFO booba;
	strcpy(booba.fname,"Booba");
	songs[2] = booba;
	num_of_songs = 3;
}

void os_init()
{	
	set_up_screen();
	os_init_scheduler();
	os_init_ruota();
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

