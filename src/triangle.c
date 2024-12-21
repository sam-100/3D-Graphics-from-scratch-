#include "triangle.h"
#include "display.h"

void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

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

