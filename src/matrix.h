#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <math.h>

typedef struct mat4_t {
    float m[4][4];
} mat4_t;

mat4_t mat4_null(void);
mat4_t mat4_make_identity(void);
mat4_t mat4_make_scaled(float sz_x, float sz_y, float sz_z);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float theta);
mat4_t mat4_make_rotation_y(float theta);
mat4_t mat4_make_rotation_z(float theta);

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4_project(mat4_t proj, vec4_t v);

#endif