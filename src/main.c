#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"

#define PI 3.14

bool is_running = false;

vec3_t camera_pos = {0, 0, 0};
uint32_t previous_time = 0;
mat4_t proj_matrix;

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

	float fov = PI/3.0;
	float aspect = (float)window_height/(float)window_width;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

	// Load the cube in our mesh data structure 
	// load_cube_mesh_data();
	load_obj_file_data("./assets/cube.obj");

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
		if( event.key.keysym.sym == SDLK_SPACE )
		{
			transform = !transform;
			break;
		}
		if( event.key.keysym.sym == SDLK_p )
		{
			painter = !painter;
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

void update(void) {
	// wait for current frame time to expire
	if(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_time+FRAME_TARGET_TIME))
		SDL_Delay(previous_time + FRAME_TARGET_TIME - SDL_GetTicks());
	previous_time = SDL_GetTicks();

	// Initialize the triangle array to NULL
	triangles_to_render = NULL;

	// Update Scale, Rotation, and Translation Values per frame.
	if(transform)
	{
		mesh.rotation.x += 0.01;
		// mesh.rotation.y += 0.01;
		// mesh.rotation.z += 0.01;

		// mesh.scale.x += 0.002;
		// mesh.scale.y += 0.002;

		// mesh.translation.x += 0.01;
		mesh.translation.z = 5;
	}
	


	// Create a scale matrix, rotation, translation, and projection matrices used to multiply mesh vertices
	mat4_t scale_matrix = mat4_make_scaled(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);


	// Load all triangles to render
	int num_mesh_faces = array_length(mesh.faces);
	for(int i=0; i<num_mesh_faces; i++)
	{
		face_t face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a-1];
		face_vertices[1] = mesh.vertices[face.b-1];
		face_vertices[2] = mesh.vertices[face.c-1];

		// Apply the transformatiosn for each vertex of the face
		vec4_t transformed_vertices[3];
		for(int j=0; j<3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Create a World matrix
            mat4_t world_matrix = mat4_make_identity();
            world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
            world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
            world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

            // Apply transformations to the mesh
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);            

			// Save the transformed vertex
			transformed_vertices[j] = transformed_vertex;
		}

		

		// backface_culling 
		if(cull_method == CULL_BACKFACE)
		{
			vec3_t camera_ray = vec3_sub(camera_pos, vec3_from_vec4(transformed_vertices[0]));
			vec3_t normal = vec3_cross(
				vec3_sub(vec3_from_vec4(transformed_vertices[1]), vec3_from_vec4(transformed_vertices[0])), 
				vec3_sub(vec3_from_vec4(transformed_vertices[2]), vec3_from_vec4(transformed_vertices[0]))
			);
			vec3_normalize(&camera_ray);
			vec3_normalize(&normal);
			if(vec3_dot(camera_ray, normal) < 0)
				continue;
		}

		// Find the avg_depth of face
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z)/3;

		// Project all vertices of current triangle
		vec4_t projected_vertices[3];
		for(int j=0; j<3; j++)
		{
			// Project and translate
			projected_vertices[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]); 	// Main call in this for loop 

			// Scale into the view
			projected_vertices[j].x *= window_width/2.0;
			projected_vertices[j].y *= window_height/2.0; 

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
	if(!painter)
		return;
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
			draw_triangle(triangle, 0xffff0000);
			for(int j=0; j<3; j++)
				draw_rect(triangle.vertices[j].x, triangle.vertices[j].y, 5, 5, 0xffff00ff);
			break;
		case RENDER_WIRE:
			draw_triangle(triangle, 0xffff0000);
			break;
		case RENDER_FILL_TRIANGLE:
			draw_filled_triangle(triangle, 0xffaaaa00);
			break;
		case RENDER_FILL_TRIANGLE_WIRE:
			draw_filled_triangle(triangle, 0xffaaaa00);
			draw_triangle(triangle, 0xffff0000);
			break;
		}
		// draw_filled_triangle(triangle, 0xffaaaa00);
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

