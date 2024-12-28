#include "triangle.h"
#include "display.h"
#include "light.h"

void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fill_flat_bottom_triangle(vec2_t a, vec2_t b, vec2_t c, uint32_t color) {
    vec2_t p = b, q = c;
    float m1, m2;
    m1 = (b.y-a.y)/(b.x-a.x);
    m2 = (c.y-a.y)/(c.x-a.x);

    while(p.y > a.y)
    {
        draw_line(p, q, color);
        
        p.y--;
        p.x = a.x + (p.y-a.y)/m1;
        
        q.y--;
        q.x = a.x + (q.y-a.y)/m2;
    }
}
void fill_flat_top_triangle(vec2_t a, vec2_t b, vec2_t c, uint32_t color) {
    vec2_t p = a, q = b;
    float m1, m2;
    m1 = (a.y-c.y)/(a.x-c.x);
    m2 = (b.y-c.y)/(b.x-c.x);

    while(p.y < c.y)
    {
        draw_line(p, q, color);

        p.y++;
        p.x = c.x + (p.y-c.y)/m1;
        
        q.y++;
        q.x = c.x + (q.y-c.y)/m2;
    }
}

void draw_filled_triangle(triangle_t triangle, uint32_t color) {
    vec2_t a, b, c;
    a = triangle.vertices[0];
    b = triangle.vertices[1];
    c = triangle.vertices[2];

    // 1. Sort the vertices based on y coordinate
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    if(b.y > c.y) 
    {
        swap(&b.x, &c.x);
        swap(&b.y, &c.y);
    }
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    

    // 2. Find Midpoint
    vec2_t mid;
    mid.y = b.y;
    mid.x = a.x + (b.y-a.y)*(c.x-a.x)/(c.y-a.y);

    // 3. Fill flat bottom triangle
    fill_flat_bottom_triangle(a, b, mid, color);
    
    // 4. Fill flat top triangle
    fill_flat_top_triangle(b, mid, c, color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flat shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_filled_triangle_flat(triangle_t triangle, uint32_t color) {
    vec2_t a, b, c;
    a = triangle.vertices[0];
    b = triangle.vertices[1];
    c = triangle.vertices[2];

    // 1. Sort the vertices based on y coordinate
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    if(b.y > c.y) 
    {
        swap(&b.x, &c.x);
        swap(&b.y, &c.y);
    }
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    

    // 2. Find Midpoint
    vec2_t mid;
    mid.y = b.y;
    mid.x = a.x + (b.y-a.y)*(c.x-a.x)/(c.y-a.y);

    // Find the new color
    float intensity = -vec3_dot(triangle.face_normal, light.direction) / (vec3_length(triangle.face_normal) * vec3_length(light.direction) );
    intensity = intensity > 0.2 ? intensity : 0.2;
    uint32_t new_color = light_apply_intensity(color, intensity);

    // 3. Fill flat bottom triangle
    fill_flat_bottom_triangle(a, b, mid, new_color);
    
    // 4. Fill flat top triangle
    fill_flat_top_triangle(b, mid, c, new_color);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Goroud shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fill_flat_bottom_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color) {

}

void fill_flat_top_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color) {

}


void draw_filled_triangle_goroud(triangle_t triangle, uint32_t color) {
    vec2_t a, b, c;
    a = triangle.vertices[0];
    b = triangle.vertices[1];
    c = triangle.vertices[2];

    vec3_t normal_a, normal_b, normal_c;
    normal_a = triangle.normals[0];
    normal_b = triangle.normals[1];
    normal_c = triangle.normals[2];

    // 1. Sort the vertices based on y coordinate
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    if(b.y > c.y) 
    {
        swap(&b.x, &c.x);
        swap(&b.y, &c.y);
    }
    if(a.y > b.y)
    {
        swap(&a.x, &b.x);
        swap(&a.y, &b.y);
    }
    

    // 2. Find Midpoint
    vec2_t mid;
    mid.y = b.y;
    mid.x = a.x + (b.y-a.y)*(c.x-a.x)/(c.y-a.y);

    // TODO: find normal at midpoint
    float t = 3.0;
    vec3_t normal_mid = {
        .x = normal_a.x + (normal_b.x - normal_a.x) * t, 
        .y = normal_a.y + (normal_b.y - normal_a.y) * t, 
        .z = normal_a.z + (normal_b.z - normal_a.z) * t  
    };

    // 3. Fill flat bottom triangle
    fill_flat_bottom_triangle_goroud(a, b, mid, normal_a, normal_b, normal_mid, color);
    
    // 4. Fill flat top triangle
    fill_flat_top_triangle_goroud(b, mid, c, normal_b, normal_mid, normal_c, color);
}
