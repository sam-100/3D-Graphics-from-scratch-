#include "vector.h"
#include <math.h>

vec3_t vec3_rotate_x(vec3_t v, float theta) {
    vec3_t rotated_vector = {
        .x = v.x, 
        .y = v.y*cos(theta) - v.z*sin(theta), 
        .z = v.y*sin(theta) + v.z*cos(theta)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float theta) {
    vec3_t rotated_vector = {
        .x = v.x*cos(theta) - v.z*sin(theta), 
        .y = v.y, 
        .z = v.x*sin(theta) + v.z*cos(theta)
    };
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float theta) {
    vec3_t rotated_vector = {
        .x = v.x*cos(theta) - v.y*sin(theta), 
        .y = v.x*sin(theta) + v.y*cos(theta), 
        .z = v.z
    };
    return rotated_vector;
}
