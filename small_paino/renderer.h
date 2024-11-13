#pragma once
#include "Windows.h"

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};
// declare the global variable so that other files may access it
// since it's cross-file variable don't add global_variable prefix
extern Render_State render_state;

void clear_screen(unsigned int color);
void draw_rect_in_pixels(int x0, int y0, int x1, int y1, unsigned int color);
void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color);