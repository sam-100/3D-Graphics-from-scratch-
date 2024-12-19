#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9*9*9)

bool is_running = false;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
int fov_factor = 640;
vec3_t camera_pos = {0, 0, -5};
vec3_t cube_rotation = {0, 0, 0};
uint32_t previous_time = 0;

// Allocates color buffer and texture
void setup(void) {
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, 
		window_width, 
		window_height
	);

	// Load Cube points in the array
	int point_num = 0;
	for(float x=-1; x <= 1; x += 0.25)
		for(float y=-1; y <= 1; y += 0.25)
			for(float z=-1; z <= 1; z += 0.25)
			{
				vec3_t new_point = {x, y, z};
				cube_points[point_num++] = new_point;
			}
}

// processes all inputs
void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if( event.key.keysym.sym == SDLK_ESCAPE )
			is_running = false;
		if( event.key.keysym.sym == SDLK_RIGHT )
			camera_pos.x += 0.1;
		if( event.key.keysym.sym == SDLK_LEFT )
			camera_pos.x -= 0.1;
		if( event.key.keysym.sym == SDLK_DOWN )
			camera_pos.z -= 0.1;
		if( event.key.keysym.sym == SDLK_UP )
			camera_pos.z += 0.1;
		if( event.key.keysym.sym == SDLK_d )
			rect_width += 10;
		if( event.key.keysym.sym == SDLK_a )
			rect_width -= 10;
		if( event.key.keysym.sym == SDLK_w )
			rect_height -= 10;
		if( event.key.keysym.sym == SDLK_s )
			rect_height += 10;
		break;
	}
}

vec2_t project(vec3_t point) {
    vec2_t projected_point = {point.x*fov_factor/point.z, point.y*fov_factor/point.z};
    return projected_point;
}

void update(void) {
	// wait for current frame time to expire
	if(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_time+FRAME_TARGET_TIME))
		SDL_Delay(previous_time + FRAME_TARGET_TIME - SDL_GetTicks());
	previous_time = SDL_GetTicks();

	// update rotation angles of cube
	cube_rotation.y += 0.01;
	cube_rotation.x += 0.01;
	cube_rotation.z += 0.01;

	// Project all points
	for(int i=0; i<N_POINTS; i++)
	{

		// Rotate the point
		vec3_t point = cube_points[i];
		vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
		transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
		

		// Translate the point 
		transformed_point.z -= camera_pos.z;
		transformed_point.x -= camera_pos.x;
		transformed_point.y -= camera_pos.y;

		// Project the point to screen
		projected_points[i] = project(transformed_point);
	}
}


// Render things on the screen
void render(void) {
	draw_grid();
	// Drawing projected points on the screen
	for(int i=0; i<N_POINTS; i++)
		draw_rect(
			projected_points[i].x + window_width/2, 
			projected_points[i].y + window_height/2, 
			4, 
			4, 
			0xffff00ff
		);

	render_color_buffer();
	clear_color_buffer(0xff000000);

	SDL_RenderPresent(renderer);
}

int main(void) {
	is_running = initialize_window();

	setup();

	while(is_running)
	{
		process_input();
		update();
		render();
	}
	
	destroy_window();
	return 0;
}





