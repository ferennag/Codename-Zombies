#include <string.h>
#include "cglm/cglm.h"
#include "data/vector.h"

Vec3 Vec3_zeroes()
{
    Vec3 result;
    result.x = 0;
    result.y = 0;
    result.z = 0;
    return result;
}

Vec3 Vec3_ones()
{
    Vec3 result;
    result.x = 1;
    result.y = 1;
    result.z = 1;
    return result;
}

Vec3 Vec3_up()
{
    Vec3 result;
    result.x = 0;
    result.y = 1;
    result.z = 0;
    return result;
}


Vec3 Vec3_init(float x, float y, float z)
{
    Vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

Vec3 Vec3_copy(Vec3 *other)
{
    Vec3 result;
    memcpy(result.xyz, other->xyz, 3 * sizeof(float));
    return result;
}

Vec3 Vec3_sub(Vec3 *a, Vec3 *b)
{
    Vec3 result;
    glm_vec3_sub(a->xyz, b->xyz, result.xyz);
    return result;
}

Vec3 Vec3_normalize(Vec3 *a)
{
    Vec3 result;
    glm_normalize_to(a->xyz, result.xyz);
    return result;
}

float Vec3_dot(Vec3 *a, Vec3 *b)
{
    return glm_vec3_dot(a->xyz, b->xyz);
}

Vec3 Vec3_cross(Vec3 *a, Vec3 *b)
{
    Vec3 result;
    glm_vec3_cross(a->xyz, b->xyz, result.xyz);
    return result;
}