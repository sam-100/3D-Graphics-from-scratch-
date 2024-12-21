#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "stdint.h"

typedef struct {
    int a, b, c;
    uint32_t color;
} face_t;

typedef struct {
    vec2_t vertices[3];
    uint32_t color;
    float avg_depth;
} triangle_t;

void draw_filled_triangle(triangle_t triangle, uint32_t color);
void fill_flat_bottom_triangle(vec2_t a, vec2_t b, vec2_t c, uint32_t color);
void fill_flat_top_triangle(vec2_t a, vec2_t b, vec2_t c, uint32_t color);


#endif
