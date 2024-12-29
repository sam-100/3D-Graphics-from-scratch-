#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"
#include "texture.h"

typedef struct {
    int a, b, c;
    tex2_t a_uv, b_uv, c_uv;
    uint32_t color;
    int normal_a, normal_b, normal_c;
} face_t;

typedef struct {
    vec2_t vertices[3];
    tex2_t texcoords[3];
    uint32_t color;
    float avg_depth;
    vec3_t normals[3];
    vec3_t face_normal;
} triangle_t;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_filled_triangle(triangle_t triangle, uint32_t color);
void fill_flat_bottom_triangle(triangle_t triangle, uint32_t color);
void fill_flat_top_triangle(triangle_t triangle, uint32_t color);

void draw_textured_triangle(triangle_t triangle, uint32_t *texture);
void fill_flat_bottom_textured_triangle(triangle_t triangle, uint32_t *texture);
void fill_flat_top_textured_triangle(triangle_t triangle, uint32_t *texture);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flat shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_filled_triangle_flat(triangle_t triangle, uint32_t color);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Goroud shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_filled_triangle_goroud(triangle_t triangle, uint32_t color);
void fill_flat_bottom_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color);
void fill_flat_top_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color);


#endif