#include "../lcd/bresenham.h"
#include "../audio/audio.h"
#include <util/delay.h>

#define CURSOR_VISUAL_OFFSET_Y 15
#define CURSOR_VISUAL_OFFSET_X 4
#define BUTTON_SCALE 0.5
#define BUTTON_Y 195
#define PAUSE_BUTTON_X 142
#define FFORWARD_BUTTON_X 210
#define FBACKWARD_BUTTON_X 110
#define MAX_SONGS 10
#define FILE_LENGTH 13


typedef enum 
{
	SONG_SELECT = 0,
	MUSIC_CONTROL
}   SCREEN_CONTEXT;

typedef enum
{
    CURSOR_DOWN = 0,
    CURSOR_UP
}   CURSOR_DIRECTION;

typedef enum
{
    BACKWARD_BUTTON = 0,
    PAUSE_BUTTON,
    FORWARD_BUTTON
}   CONTROL_CURSOR;

void draw_pause_button(int x, int y, float scale);
void draw_pause_button_selected(int x, int y, float scale);
void draw_fforward_button(int x, int y, float scale);
void draw_fforward_button_selected(int x, int y, float scale);
void draw_fbackward_button(int x, int y, float scale);
void draw_fbackward_button_selected(int x, int y, float scale);
void draw_horizontal_delimiter(int y);
void draw_rectangle(int x1, int x2, int y1, int y2, uint16_t col);
void draw_no_song_playing(const SCREEN_CONTEXT const* context);
void draw_song_playing(const TCHAR const song[FILE_LENGTH], const SCREEN_CONTEXT const* context);
void draw_music_controls();
void hide_music_controls();
void draw_updated_cursor(const TCHAR const prev_song[FILE_LENGTH], const TCHAR const next_song[FILE_LENGTH], uint8_t y_pos_nxt, uint8_t y_pos_prev);
void update_control_cursor(const CONTROL_CURSOR const* cursor);
void boot_screen_animation();
void display_home_screen(const uint8_t const* num_of_songs, TCHAR songs[MAX_SONGS][FILE_LENGTH]);



