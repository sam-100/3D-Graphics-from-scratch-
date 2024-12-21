#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

bool is_running = false;

int fov_factor = 640;
vec3_t camera_pos = {0, 0, 0};
uint32_t previous_time = 0;

triangle_t *triangles_to_render = NULL;

void setup(void) {
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, 
		window_width, 
		window_height
	);

	// Load the cube in our mesh data structure 
	load_cube_mesh_data();
	// load_obj_file_data("./assets/cube.obj");

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
		{
			is_running = false;
			break;
		}
		if( event.key.keysym.sym == SDLK_1 )
		{
			render_method = RENDER_WIRE_VERTEX;
			break;
		}
		if( event.key.keysym.sym == SDLK_2 )
		{
			render_method = RENDER_WIRE;
			break;
		}
		if( event.key.keysym.sym == SDLK_3 )
		{
			render_method = RENDER_FILL_TRIANGLE;
			break;
		}
		if( event.key.keysym.sym == SDLK_4 )
		{
			render_method = RENDER_FILL_TRIANGLE_WIRE;
			break;
		}
		if( event.key.keysym.sym == SDLK_c )
		{
			cull_method = CULL_BACKFACE;
			break;
		}
		if( event.key.keysym.sym == SDLK_d )
		{
			cull_method = CULL_NONE;
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		printf("Mouse click down at <%d, %d>\n", event.button.x, event.button.y);
		break;
	case SDL_MOUSEBUTTONUP:
		printf("Mouse click up at <%d, %d>\n", event.button.x, event.button.y);
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

	// Initialize the triangle array to NULL
	triangles_to_render = NULL;

	// Rotate the cube
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	// Load all triangles to render
	int num_mesh_faces = array_length(mesh.faces);
	for(int i=0; i<num_mesh_faces; i++)
	{
		face_t face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a-1];
		face_vertices[1] = mesh.vertices[face.b-1];
		face_vertices[2] = mesh.vertices[face.c-1];

		// Rotate and Transform the vertices
		vec3_t transformed_vertices[3];
		for(int i=0; i<3; i++)
		{
			vec3_t transformed_vertex = face_vertices[i];

			// Rotate the face
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			// Translate the vertex away from camera
			transformed_vertex.z += 5;

			transformed_vertices[i] = transformed_vertex;
		}

		// backface_culling 
		if(cull_method == CULL_BACKFACE)
		{
			vec3_t camera_ray = vec3_sub(camera_pos, transformed_vertices[0]);
			vec3_t normal = vec3_cross(
				vec3_sub(transformed_vertices[1], transformed_vertices[0]), 
				vec3_sub(transformed_vertices[2], transformed_vertices[0])
			);
			vec3_normalize(&camera_ray);
			vec3_normalize(&normal);
			if(vec3_dot(camera_ray, normal) < 0)
				continue;
		}

		// Find the avg_depth of face
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z)/3;

		// Project all vertices of current triangle
		vec2_t projected_vertices[3];
		for(int j=0; j<3; j++)
		{
			// Project and translate
			projected_vertices[j] = project(transformed_vertices[j]);			 // Main call in this for loop 

			// Translate vertex to middle of the screen
			projected_vertices[j].x += window_width/2;
			projected_vertices[j].y += window_height/2;
		}

		// Push projected triangle to triangles_to_render array
		triangle_t projected_triangle = {
			.vertices = {
				{projected_vertices[0].x, projected_vertices[0].y}, 
				{projected_vertices[1].x, projected_vertices[1].y}, 
				{projected_vertices[2].x, projected_vertices[2].y}
			}, 
			.color = face.color, 
			.avg_depth = avg_depth
		};
		array_push(triangles_to_render, projected_triangle);
	}

	// Sort all triangles to render in dsc order of their avg_depth
	int num_triangles = array_length(triangles_to_render);
	for(int i=0; i<num_triangles; i++)
		for(int j=0; j<num_triangles-i-1; j++)
		{
			if(triangles_to_render[j+1].avg_depth > triangles_to_render[j].avg_depth)
			{
				// swap
				triangle_t temp;
				temp = triangles_to_render[j];
				triangles_to_render[j] = triangles_to_render[j+1];
				triangles_to_render[j+1] = temp;
			}
		}
}

uint32_t colors[6] = {0xffff0000, 0xff00ff00, 0xff0000ff, 0xffffff00, 0xffff00ff, 0xff00ffff};

// Render things on the screen
void render(void) {
	draw_grid();

	// Drawing projected points on the screen
	int num_triangles = array_length(triangles_to_render);
	for(int i=0; i<num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		switch(render_method)
		{
		case RENDER_WIRE_VERTEX:
			draw_triangle(triangle, 0xffffffff);
			for(int j=0; j<3; j++)
				draw_rect(triangle.vertices[j].x, triangle.vertices[j].y, 5, 5, 0xffff00ff);
			break;
		case RENDER_WIRE:
			draw_triangle(triangle, 0xffffffff);
			break;
		case RENDER_FILL_TRIANGLE:
			draw_filled_triangle(triangle, triangle.color);
			break;
		case RENDER_FILL_TRIANGLE_WIRE:
			draw_filled_triangle(triangle, triangle.color);
			draw_triangle(triangle, 0xffffffff);
			break;
		}
		// draw_filled_triangle(triangle, 0xffffff00);
		// draw_triangle(triangle, 0x00000000);
	}

	// Deallocate the triangle array
	array_free(triangles_to_render);

	// Clear the buffer
	render_color_buffer();
	clear_color_buffer(0xff000000);

	// Present renderer to screen
	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(color_buffer);
	array_free(mesh.vertices);
	array_free(mesh.faces);
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
	free_resources();
	return 0;
}

