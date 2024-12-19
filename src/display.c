#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture *color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;
int rect_x = 200, rect_y = 100;
int rect_width = 100, rect_height = 100;

bool initialize_window(void) {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL\n");
		return false;
	}

	// Query SDL to get the display size
	SDL_DisplayMode display_mode;
	SDL_GetDisplayMode(0, 0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create SDL window
	window = SDL_CreateWindow("Hello Window!", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		window_width, 
		window_height, 
		SDL_WINDOW_BORDERLESS
	);
	if(!window)
	{
		fprintf(stderr, "Error Creating the window\n");
		return false;
	}

	// Create SDL Renderer context
	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer)
	{
		fprintf(stderr, "Error creating renderer\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyTexture(color_buffer_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void clear_color_buffer(uint32_t color) {
	for(int y=0; y<window_height; y++)
		for(int x=0; x<window_width; x++)
			color_buffer[y*window_width + x] = color;
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture, 
		NULL, 
		color_buffer, 
		sizeof(uint32_t) * window_width
	);
	SDL_RenderCopy(
		renderer, 
		color_buffer_texture, 
		NULL, 
		NULL
	);
}

void draw_grid(void) {
	for(int y=0; y<window_height; y += 10)
		for(int x=0; x<window_width; x += 10)
				color_buffer[y*window_width + x] = 0xff555555;
}

void draw_pixel(int x, int y, uint32_t color) {
    if( x < 0 || x > window_width)
        return;
    if( y < 0 || y > window_height)
        return;
    color_buffer[y*window_width+x] = color;
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
	for(int j=x; j<x+width; j++)
		for(int i=y; i<y+height; i++)
			draw_pixel(j, i, color);
}

