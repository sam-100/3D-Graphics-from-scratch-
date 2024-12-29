#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "triangle.h"
#include "matrix.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern uint32_t *color_buffer;
extern SDL_Texture *color_buffer_texture;

extern int window_width;
extern int window_height;

enum cull_method {
    CULL_NONE, 
    CULL_BACKFACE
};
extern enum cull_method cull_method;

enum render_method {
    RENDER_WIRE, 
    RENDER_WIRE_VERTEX, 
    RENDER_FILL_TRIANGLE_WIRE, 
    RENDER_FILL_TRIANGLE, 
    RENDER_FILL_TRIANGLE_FLAT, 
    RENDER_TEXTURE_TRIANGLE, 
    RENDER_TEXTURE_TRIANGLE_WIRE, 
    TEST, 
};
extern enum render_method render_method;

extern bool transform;
extern bool painter;

extern mat4_t proj_matrix;

bool initialize_window(void);
void destroy_window(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_line(vec2_t a, vec2_t b, uint32_t color);
void draw_triangle(triangle_t triangle, uint32_t color);
void draw_normals(triangle_t triangle, vec2_t *normals, uint32_t color);
vec4_t project_point_to_screen(mat4_t proj_matrix, vec4_t point);

#endif