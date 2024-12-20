#include "mesh.h"
#include "triangle.h"

vec3_t cube_vertices[N_MESH_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y =  1, .z = -1}, // 2
    {.x =  1, .y =  1, .z = -1}, // 3
    {.x =  1, .y = -1, .z = -1}, // 4
    {.x =  1, .y =  1, .z =  1}, // 5
    {.x =  1, .y = -1, .z =  1}, // 6
    {.x = -1, .y =  1, .z =  1}, // 7
    {.x = -1, .y = -1, .z =  1}, // 8
};

face_t cube_faces[N_MESH_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3}, 
    {.a = 3, .b = 4, .c = 1}, 

    // back
    {.a = 5, .b = 7, .c = 8}, 
    {.a = 5, .b = 8, .c = 6}, 

    // right 
    {.a = 3, .b = 5, .c = 4}, 
    {.a = 5, .b = 6, .c = 4}, 

    // left 
    {.a = 7, .b = 1, .c = 2}, 
    {.a = 1, .b = 8, .c = 7}, 

    // top 
    {.a = 2, .b = 7, .c = 5}, 
    {.a = 2, .b = 5, .c = 3}, 

    // bottom
    {.a = 1, .b = 4, .c = 6}, 
    {.a = 1, .b = 6, .c = 8}, 
};
