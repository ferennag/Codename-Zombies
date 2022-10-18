#include <stdlib.h>
#include "data/vector.h"
#include "engine/camera.h"

Camera *Camera_create()
{
    Camera *cam = (Camera *) malloc(sizeof(Camera));

    cam->speed = 0.5f;
    cam->yaw = -90.0f;
    cam->pitch = 0.0f;
    cam->position = Vec3_init(0.0, 0.0, 3.0);

    glm_mat4_identity(cam->view);

    // glm_look
    return cam;
}

void Camera_destroy(Camera *cam)
{
    free(cam);
}

Vec3 Camera_direction(Camera *cam) {
    Vec3 direction;
    direction.x = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    direction.y = -sin(glm_rad(cam->pitch));
    direction.z = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
    glm_vec3_normalize(direction.xyz);
    return direction;
}

Vec3 Camera_right(Camera *cam) {
    Vec3 up = Vec3_up();
    Vec3 dir = Camera_direction(cam);
    Vec3 right = Vec3_cross(&up, &dir);
    return Vec3_normalize(&right);
}

Vec3 Camera_up(Camera *cam) {
    Vec3 right = Camera_right(cam);
    Vec3 dir = Camera_direction(cam);
    Vec3 up = Vec3_cross(&dir, &right);
    return up;
}

void Camera_lookat(Camera *cam)
{
    Vec3 up = Camera_up(cam);
    Vec3 front;
    glm_vec3_add(cam->position.xyz, Camera_direction(cam).xyz, front.xyz);
    glm_lookat(cam->position.xyz, front.xyz, up.xyz, cam->view);
}

void Camera_move_forward(Camera *cam, float value) {
    glm_vec3_muladds(Camera_direction(cam).xyz, cam->speed, cam->position.xyz);
}

void Camera_move_backward(Camera *cam, float value) {
    glm_vec3_muladds(Camera_direction(cam).xyz, -cam->speed, cam->position.xyz);
}

void Camera_strafe_left(Camera *cam, float value)
{
    Vec3 right = Camera_right(cam);
    glm_vec3_add(cam->position.xyz, right.xyz, cam->position.xyz);
}

void Camera_strafe_right(Camera *cam, float value)
{
    Vec3 right = Camera_right(cam);
    glm_vec3_sub(cam->position.xyz, right.xyz, cam->position.xyz);
}