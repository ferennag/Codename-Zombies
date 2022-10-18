#pragma once

#include <data/vector.h>

typedef struct Camera {
    float speed;
    float yaw;
    float pitch;
    Vec3 position;
    mat4 view;
} Camera;

Camera *Camera_create();
void Camera_destroy(Camera *cam);
void Camera_lookat(Camera *cam);

void Camera_move_forward(Camera *cam, float value);
void Camera_move_backward(Camera *cam, float value);

void Camera_strafe_left(Camera *cam, float value);
void Camera_strafe_right(Camera *cam, float value);