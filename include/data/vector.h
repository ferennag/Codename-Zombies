#pragma once

#include "cglm/cglm.h"

typedef union Vec3 {
    struct {
        float x, y, z;
    };
    vec3 xyz;
} Vec3;

Vec3 Vec3_zeroes();
Vec3 Vec3_ones();
Vec3 Vec3_up();
Vec3 Vec3_init(float x, float y, float z);
Vec3 Vec3_copy(Vec3 *other);

Vec3 Vec3_sub(Vec3 *a, Vec3 *b);
Vec3 Vec3_normalize(Vec3 *a);

float Vec3_dot(Vec3 *a, Vec3 *b);
Vec3 Vec3_cross(Vec3 *a, Vec3 *b);