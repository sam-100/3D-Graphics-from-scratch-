#include "triangle.h"
#include "display.h"
#include "light.h"

#define MIN_INTENSITY 0.4

void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

float lerp(float a, float b, float x, float y, float m) {
    return a + (b-a)*(m-x)/(y-x);
}

uint32_t get_texture_color_at(uint32_t *texture, tex2_t pos) {
    // Todo: 
    uint8_t *texture_array = (uint8_t*)texture;
    int x, y;
    x = pos.u*texture_width;
    y = pos.v*texture_height;
    uint8_t color_8 = texture_array[y*texture_width + x];
    uint32_t color_32;
    uint8_t a, r, g, b;
    // a_2 = (color_8 & 0b11000000);
    // a_8 = ;

    color_32 = (a << 24) + (r << 16) + (g << 8) + b;
    return 0xff0000ff;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// No shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fill_flat_bottom_triangle(triangle_t triangle, uint32_t color) {
    vec2_t p_vert = triangle.vertices[1], q_vert = triangle.vertices[2];
    float m1, m2;
    m1 = (triangle.vertices[1].y-triangle.vertices[0].y)/(triangle.vertices[1].x-triangle.vertices[0].x);
    m2 = (triangle.vertices[2].y-triangle.vertices[0].y)/(triangle.vertices[2].x-triangle.vertices[0].x);

    while(p_vert.y > triangle.vertices[0].y)
    {
        draw_line(p_vert, q_vert, color);
        
        p_vert.y--;
        p_vert.x = triangle.vertices[0].x + (p_vert.y-triangle.vertices[0].y)/m1;
        
        q_vert.y--;
        q_vert.x = triangle.vertices[0].x + (q_vert.y-triangle.vertices[0].y)/m2;
    }
}

void fill_flat_top_triangle(triangle_t triangle, uint32_t color) {
    vec2_t p_vert = triangle.vertices[0], q_vert = triangle.vertices[1];
    float m1, m2;
    m1 = (triangle.vertices[0].y-triangle.vertices[2].y)/(triangle.vertices[0].x-triangle.vertices[2].x);
    m2 = (triangle.vertices[1].y-triangle.vertices[2].y)/(triangle.vertices[1].x-triangle.vertices[2].x);

    while(p_vert.y < triangle.vertices[2].y)
    {
        draw_line(p_vert, q_vert, color);

        p_vert.y++;
        p_vert.x = triangle.vertices[2].x + (p_vert.y-triangle.vertices[2].y)/m1;
        
        q_vert.y++;
        q_vert.x = triangle.vertices[2].x + (q_vert.y-triangle.vertices[2].y)/m2;
    }
}

void draw_filled_triangle(triangle_t triangle, uint32_t color) {
    int index[3] = {0, 1, 2};

    // 1. Sort the vertices based on y coordinate
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    if(triangle.vertices[index[1]].y > triangle.vertices[index[2]].y) 
    {
        swap_int(&index[1], &index[2]);
    }
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    

    // 2. Find Midpoint
    vec2_t mid_vertex;
    mid_vertex.y = triangle.vertices[index[1]].y;
    mid_vertex.x = triangle.vertices[index[0]].x + (triangle.vertices[index[1]].y-triangle.vertices[index[0]].y) * (triangle.vertices[index[2]].x-triangle.vertices[index[0]].x) / (triangle.vertices[index[2]].y-triangle.vertices[index[0]].y);

    // 3. Fill flat bottom triangle
    triangle_t flat_bottom_triangle = {
        .vertices = { triangle.vertices[index[0]], triangle.vertices[index[1]], mid_vertex}, 
    };
    fill_flat_bottom_triangle(flat_bottom_triangle, color); // (a, b, mid, color);
    
    // 4. Fill flat top triangle
    triangle_t flat_top_triangle = {
        .vertices = {triangle.vertices[index[1]], mid_vertex, triangle.vertices[index[2]]}, 
    };
    fill_flat_top_triangle(flat_top_triangle, color); // (b, mid, c, color);
}

void draw_textured_triangle(triangle_t triangle, uint32_t *texture) {
    int index[3] = {0, 1, 2};

    // 1. Sort the vertices based on y coordinate
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    if(triangle.vertices[index[1]].y > triangle.vertices[index[2]].y) 
    {
        swap_int(&index[1], &index[2]);
    }
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    

    // 2. Find Midpoint
    vec2_t mid_vertex;
    mid_vertex.x = triangle.vertices[index[0]].x + (triangle.vertices[index[1]].y-triangle.vertices[index[0]].y) * (triangle.vertices[index[2]].x-triangle.vertices[index[0]].x) / (triangle.vertices[index[2]].y-triangle.vertices[index[0]].y);
    mid_vertex.y = triangle.vertices[index[1]].y;
    

    // Find the texture coordinates of mid point using linear interpolation  
    tex2_t mid_texcoords;
    mid_texcoords.u = lerp(triangle.texcoords[index[0]].u, triangle.texcoords[index[2]].u, triangle.vertices[index[0]].x, triangle.vertices[index[2]].x, mid_vertex.x);
    mid_texcoords.v = lerp(triangle.texcoords[index[0]].v, triangle.texcoords[index[2]].v, triangle.vertices[index[0]].y, triangle.vertices[index[2]].y, mid_vertex.y);

    // 3. Fill flat bottom triangle
    if(triangle.vertices[index[0]].y < mid_vertex.y)
    {
        triangle_t flat_bottom_triangle = {
            .vertices = { triangle.vertices[index[0]], triangle.vertices[index[1]], mid_vertex}, 
            .texcoords = { triangle.texcoords[index[0]], triangle.texcoords[index[1]], mid_texcoords}
        };
        fill_flat_bottom_textured_triangle(flat_bottom_triangle, texture); // (a, b, mid, color);
    }
    
    // 4. Fill flat top triangle
    if(triangle.vertices[index[2]].y > mid_vertex.y)
    {
        triangle_t flat_top_triangle = {
            .vertices = {triangle.vertices[index[1]], mid_vertex, triangle.vertices[index[2]]}, 
            .texcoords = {triangle.texcoords[index[1]], mid_texcoords, triangle.texcoords[index[2]]}
        };
        fill_flat_top_textured_triangle(flat_top_triangle, texture); // (b, mid, c, color); 
    }
}

void fill_flat_bottom_textured_triangle(triangle_t triangle, uint32_t *texture) {
    vec2_t p_vert, q_vert;
    p_vert = triangle.vertices[1];
    q_vert = triangle.vertices[2];

    tex2_t p_tex, q_tex;
    p_tex = triangle.texcoords[1];
    q_tex = triangle.texcoords[2];

    float m1, m2;
    m1 = (triangle.vertices[1].y-triangle.vertices[0].y)/(triangle.vertices[1].x-triangle.vertices[0].x);
    m2 = (triangle.vertices[2].y-triangle.vertices[0].y)/(triangle.vertices[2].x-triangle.vertices[0].x);

    uint32_t vert_colors[3];
    vert_colors[0] = get_texture_color_at(texture, triangle.texcoords[0]);
    vert_colors[1] = get_texture_color_at(texture, triangle.texcoords[1]); 
    vert_colors[2] = get_texture_color_at(texture, triangle.texcoords[2]);


    while(p_vert.y > triangle.vertices[0].y)
    {
        // draw_line(p_vert, q_vert, color);
        // Todo: draw_line_interp(p_vert, q_vert, mesh_texture[], mesh_texture[]);
        uint32_t p_color, q_color;
        p_color = get_texture_color_at(texture, p_tex);
        q_color = get_texture_color_at(texture, q_tex);
        uint32_t color = p_color;
        tex2_t tex= p_tex;
        for(int y=p_vert.y, x = p_vert.x; y <= q_vert.y; y++)
        {
            tex.u = lerp(p_tex.u, q_tex.u, p_vert.y, q_vert.y, y);
            color = get_texture_color_at(texture, tex);
            draw_pixel(x, y, color);

        }
        
        // update vertices
        p_vert.y--;
        p_vert.x = triangle.vertices[0].x + (p_vert.y-triangle.vertices[0].y)/m1;
        q_vert.y--;
        q_vert.x = triangle.vertices[0].x + (q_vert.y-triangle.vertices[0].y)/m2;
        
        // update texture coordinates
        p_tex.u = lerp(triangle.texcoords[0].u, triangle.texcoords[1].u, triangle.vertices[0].x, triangle.vertices[1].x, p_vert.x);
        p_tex.v = lerp(triangle.texcoords[0].v, triangle.texcoords[1].v, triangle.vertices[0].y, triangle.vertices[1].y, p_vert.y);
        q_tex.u = lerp(triangle.texcoords[0].u, triangle.texcoords[2].u, triangle.vertices[0].x, triangle.vertices[2].x, q_vert.x);
        q_tex.v = lerp(triangle.texcoords[0].v, triangle.texcoords[2].v, triangle.vertices[0].y, triangle.vertices[2].y, q_vert.y);
    }
}

void fill_flat_top_textured_triangle(triangle_t triangle, uint32_t *texture) {

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flat shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_filled_triangle_flat(triangle_t triangle, uint32_t color) {
    int index[3] = {0, 1, 2};

    // 1. Sort the vertices based on y coordinate
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    if(triangle.vertices[index[1]].y > triangle.vertices[index[2]].y) 
    {
        swap_int(&index[1], &index[2]);
    }
    if(triangle.vertices[index[0]].y > triangle.vertices[index[1]].y)
    {
        swap_int(&index[0], &index[1]);
    }
    

    // 2. Find Midpoint
    vec2_t mid_vertex;
    mid_vertex.y = triangle.vertices[index[1]].y;
    mid_vertex.x = triangle.vertices[index[0]].x + (triangle.vertices[index[1]].y-triangle.vertices[index[0]].y) * (triangle.vertices[index[2]].x-triangle.vertices[index[0]].x) / (triangle.vertices[index[2]].y-triangle.vertices[index[0]].y);

    // Find the new color
    float intensity = -vec3_dot(triangle.face_normal, light.direction) / (vec3_length(triangle.face_normal) * vec3_length(light.direction) );
    intensity = intensity > MIN_INTENSITY ? intensity : MIN_INTENSITY;
    uint32_t new_color = light_apply_intensity(color, intensity);

    // 3. Fill flat bottom triangle
    triangle_t flat_bottom_triangle = {
        .vertices = { triangle.vertices[index[0]], triangle.vertices[index[1]], mid_vertex}, 
    };
    fill_flat_bottom_triangle(flat_bottom_triangle, new_color); // (a, b, mid, color);
    
    // 4. Fill flat top triangle
    triangle_t flat_top_triangle = {
        .vertices = {triangle.vertices[index[1]], mid_vertex, triangle.vertices[index[2]]}, 
    };
    fill_flat_top_triangle(flat_top_triangle, new_color); // (b, mid, c, color);



}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Goroud shading functions 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fill_flat_bottom_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color) {

}

void fill_flat_top_triangle_goroud(vec2_t a, vec2_t b, vec2_t c, vec3_t normal_a, vec3_t normal_b, vec3_t normal_c, uint32_t color) {

}


void draw_filled_triangle_goroud(triangle_t triangle, uint32_t color) {

}
