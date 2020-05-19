#include "music_gfx.h"

void draw_pause_button(int x, int y, float scale)
{
	rectangle outer_rec1 = {x, x + 24 * scale,  y , y + 64 * scale};
	rectangle inner_rec1 = {x + 2 * scale, x + 22 * scale, y + 2 * scale, y + 62 * scale};
	rectangle outer_rec2 = {x + 48 * scale,  x + 72 * scale, y, y + 64 * scale};
	rectangle inner_rec2 = {x + 50 * scale, x + 70 * scale, y + 2 * scale , y + 62 * scale};
	fill_rectangle(outer_rec1, WHITE);
	fill_rectangle(outer_rec2, WHITE);
	fill_rectangle(inner_rec1, BLACK);
	fill_rectangle(inner_rec2, BLACK);
}

void draw_pause_button_selected(int x, int y, float scale)
{
	rectangle outer_rec1 = {x, x + 24 * scale,  y , y + 64 * scale};
	rectangle outer_rec2 = {x + 48 * scale,  x + 72 * scale, y, y + 64 * scale};
	fill_rectangle(outer_rec1, WHITE);
	fill_rectangle(outer_rec2, WHITE);	
}

void draw_fforward_button(int x, int y, float scale)
{
	drawLine(x, y , x , y + 64* scale, WHITE);
	drawLine(x , y, x + 32* scale, y + 32* scale, WHITE);
	drawLine(x , y + 64* scale, x +32* scale, y +32* scale, WHITE);
	int fill_y = y + 1;
	int fill_x = x;
	int length = 62 * scale -1 ;
	while(fill_x < (x + 32 * scale + 1))
	{
		fill_x += 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, BLACK); 
	}
}

void draw_fforward_button_selected(int x, int y, float scale)
{
	drawLine(x, y , x , y + 64 * scale, WHITE);
	drawLine(x , y, x + 32* scale, y + 32* scale, WHITE);
	drawLine(x , y + 64* scale, x +32* scale, y +32* scale, WHITE);
	int fill_y = y;
	int fill_x = x;
	int length = 64* scale;
	while(fill_x < (x + 32* scale))
	{
		fill_x += 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, WHITE); 
	}
}

void draw_fbackward_button(int x, int y, float scale)
{
	drawLine(x, y , x , y + 64* scale, WHITE);
	drawLine(x , y, x - 32* scale, y + 32* scale, WHITE);
	drawLine(x , y + 64* scale, x - 32* scale, y +32* scale, WHITE);
	int fill_y = y + 1;
	int fill_x = x;
	int length = 62* scale - 1;
	while(fill_x > (x - 32* scale - 1))
	{
		fill_x -= 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, BLACK); 
	}
}

void draw_fbackward_button_selected(int x, int y, float scale)
{
	drawLine(x, y , x , y + 64* scale, WHITE);
	drawLine(x , y, x - 32* scale, y + 32* scale, WHITE);
	drawLine(x , y + 64* scale, x - 32* scale, y +32* scale, WHITE);
	int fill_y = y;
	int fill_x = x;
	int length = 64* scale;
	while(fill_x > (x - 32* scale))
	{
		fill_x -= 1;
		fill_y += 1;
		length -= 2;
		drawLine(fill_x, fill_y, fill_x, fill_y + length, WHITE); 
	}
}

void draw_horizontal_delimiter(int y)
{
	rectangle rect = {0, 319, y, y + 1};
	fill_rectangle(rect, WHITE);
}

void draw_rectangle(int x1, int x2, int y1, int y2, uint16_t col)
{
    rectangle rect = {x1, x2, y1, y2};
    fill_rectangle(rect, col);
}

// position of string "No song playing" depends on SCREEN_CONTEXT
void draw_no_song_playing(const SCREEN_CONTEXT const* context)
{
	if(*context == SONG_SELECT)
	{
	draw_rectangle(0, LCDHEIGHT, 230, LCDWIDTH, BLACK);
	display_string_xy("No song playing", 120, 230);
	draw_rectangle(0, LCDHEIGHT, 180, 190, BLACK);
	}
	else if (*context == MUSIC_CONTROL)
	{
	draw_rectangle(0, LCDHEIGHT, 180, 190, BLACK);
	display_string_xy("No song playing", 120 , 180);
	draw_rectangle(0, LCDHEIGHT, 230, LCDWIDTH, BLACK);
	}

}

// displays song playing with position depending on SCREEN_CONTEXT
void draw_song_playing(const TCHAR const song[FILE_LENGTH], const SCREEN_CONTEXT const* context)
{
	if (*context == SONG_SELECT)
	{
		draw_rectangle(0, LCDHEIGHT, 180, 190, BLACK);
		draw_rectangle(160, LCDHEIGHT, 230, LCDWIDTH, BLACK);
		char playing[] = "Playing: ";
		display_string_xy(playing, 110, 229);
		display_string_xy(song, 160, 229);
		
	}
	else if (*context == MUSIC_CONTROL)
	{
		draw_rectangle(0, LCDHEIGHT, 230, LCDWIDTH, BLACK);
		draw_rectangle(160, LCDHEIGHT, 180, 190, BLACK);
		char playing[] = "Playing: ";
		display_string_xy(playing, 110, 180);	
		display_string_xy(song, 160, 180);
	}
}

void update_control_cursor(const CONTROL_CURSOR const* cursor)
{
	if(*cursor == 0)
	{
		draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
		draw_fbackward_button_selected(FBACKWARD_BUTTON_X,BUTTON_Y ,BUTTON_SCALE);
	}
	else if (*cursor == 1)
	{
		draw_pause_button_selected(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
		draw_fforward_button(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
		draw_fbackward_button(FBACKWARD_BUTTON_X,BUTTON_Y ,BUTTON_SCALE);
	}
	else if (*cursor == 2)
	{
		draw_fforward_button_selected(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
		draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
	}
}

void draw_music_controls()
{
	rectangle rect = {0, 319, 120, 239};
	fill_rectangle(rect, BLACK);
	draw_horizontal_delimiter(175);
	draw_pause_button(PAUSE_BUTTON_X, BUTTON_Y, BUTTON_SCALE);
	draw_fforward_button(FFORWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
	draw_fbackward_button(FBACKWARD_BUTTON_X, BUTTON_Y,BUTTON_SCALE);
}

void hide_music_controls()
{
	draw_rectangle(0, LCDHEIGHT, 175, LCDWIDTH, BLACK);
	draw_horizontal_delimiter(227);
}

void draw_updated_cursor(const TCHAR const prev_song[FILE_LENGTH], const TCHAR const next_song[FILE_LENGTH], uint8_t y_pos_prev, uint8_t y_pos_nxt)
{
	display_string_xy_inverted(next_song, CURSOR_VISUAL_OFFSET_X, y_pos_nxt + CURSOR_VISUAL_OFFSET_Y);
	display_string_xy(prev_song, CURSOR_VISUAL_OFFSET_X,y_pos_prev + CURSOR_VISUAL_OFFSET_Y);
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

void display_home_screen(const uint8_t const* num_of_songs, TCHAR songs[MAX_SONGS][FILE_LENGTH])
{
	clear_screen();
	display_string_xy("FORTUNA-POD", 130, 1);
	draw_horizontal_delimiter(10);
	draw_horizontal_delimiter(227);
	for (uint8_t i = 0; i < *num_of_songs; i++)
	{
		display_string_xy(songs[i], CURSOR_VISUAL_OFFSET_X, (i*8) + CURSOR_VISUAL_OFFSET_Y);
	}
	
}