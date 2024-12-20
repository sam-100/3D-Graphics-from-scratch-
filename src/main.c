#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

int fov_factor = 640;
vec3_t camera_pos = {0, 0, -5};
uint32_t previous_time = 0;
vec3_t cube_rotation = {0, 0, 0};

triangle_t triangles_to_render[N_MESH_FACES];


void setup(void) {
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, 
		window_width, 
		window_height
	);
	
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
		break;
	}
}

vec2_t project(vec3_t point) {
	// vec2_t projected_point = {point.x*fov_factor, point.y*fov_factor};
    vec2_t projected_point = {point.x*fov_factor/point.z, point.y*fov_factor/point.z};
    return projected_point;
}

void update(void) {
	// wait for current frame time to expire
	if(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_time+FRAME_TARGET_TIME))
		SDL_Delay(previous_time + FRAME_TARGET_TIME - SDL_GetTicks());
	previous_time = SDL_GetTicks();

	// Rotate the cube
	cube_rotation.x += 0.01;
	cube_rotation.y += 0.01;
	cube_rotation.z += 0.01;

	// Load all triangles to render
	for(int i=0; i<N_MESH_FACES; i++)
	{
		face_t cube_face = cube_faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = cube_vertices[cube_face.a-1];
		face_vertices[1] = cube_vertices[cube_face.b-1];
		face_vertices[2] = cube_vertices[cube_face.c-1];

		triangle_t projected_triangle;
		// Project all vertices of current triangle
		for(int j=0; j<3; j++)
		{
			vec3_t vertex = face_vertices[j];

			// Rotate 
			vec3_t transformed_vertex = vertex;
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			// Translate the vertex away from camera
			transformed_vertex.z -= camera_pos.z;

			// Project and translate
			projected_triangle.vertices[j] = project(transformed_vertex);
			projected_triangle.vertices[j].x += window_width/2; 
			projected_triangle.vertices[j].y += window_height/2;
		}

		triangles_to_render[i] = projected_triangle;
	}
}


// Render things on the screen
void render(void) {
	draw_grid();

	// Drawing projected points on the screen
	for(int i=0; i<N_MESH_FACES; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		draw_triangle(triangle, 0xffffff00);
	}

	// Clear the buffer
	render_color_buffer();
	clear_color_buffer(0xff000000);

	// Present renderer to screen
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





