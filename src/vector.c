#include "vector.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of 2D Vector operation functions
///////////////////////////////////////////////////////////////////////////////////////////////

float vec2_length(vec2_t v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
    vec2_t result = {
        .x = a.x + b.x, 
        .y = a.y + b.y
    };
    return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
    vec2_t result = {
        .x = a.x - b.x, 
        .y = a.y - b.y
    };
    return result;
}

vec2_t vec2_mul(vec2_t v, float factor) {
    vec2_t result = {
        .x = v.x*factor, 
        .y = v.y*factor
    };
    return result;
}

vec2_t vec2_div(vec2_t v, float factor) {
    vec2_t result = {
        .x = v.x*factor, 
        .y = v.y*factor
    };
    return result;
}

void vec2_normalize(vec2_t *v) {
    float length = vec2_length(*v);
    v->x /= length;
    v->y /= length;
}

float vec2_dot(vec2_t a, vec2_t b) {
    return a.x*b.x + a.y*b.y;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of 3D Vector operation functions
///////////////////////////////////////////////////////////////////////////////////////////////

float vec3_length(vec3_t v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.x + b.x, 
        .y = a.y + b.y, 
        .z = a.z + b.z
    };
    return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.x - b.x, 
        .y = a.y - b.y, 
        .z = a.z - b.z  
    };
    return result;
}

vec3_t vec3_mul(vec3_t v, float factor) {
    vec3_t result = {
        .x = v.x*factor, 
        .y = v.y*factor, 
        .z = v.z*factor
    };
    return result;
}

vec3_t vec3_div(vec3_t v, float factor) {
    vec3_t result = {
        .x = v.x/factor, 
        .y = v.y/factor, 
        .z = v.z/factor
    };
    return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.y*b.z - a.z*b.y, 
        .y = b.x*a.z - a.x*b.z, 
        .z = a.x*b.y - b.x*a.y
    };
    return result;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

void vec3_normalize(vec3_t *v) {
    float length = vec3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vec3_t vec3_negative(vec3_t v) {
    vec3_t result = {
        .x = -v.x, 
        .y = -v.y, 
        .z = -v.z
    };
    return result;
}

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

///////////////////////////////////////////////////////////////////////////////////////////////
// Vector transformation functions
///////////////////////////////////////////////////////////////////////////////////////////////
vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t result = {v.x, v.y, v.z, 1.0};
    return result;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t result = {v.x/v.w, v.y/v.w, v.z/v.w};
    return result;
}

