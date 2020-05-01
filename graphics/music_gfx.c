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