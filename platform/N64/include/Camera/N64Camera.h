#ifndef N64CAMERA_H
#define N64CAMERA_H

#include <t3d/t3d.h>

struct N64Camera{
    fm_vec3_t transform;
    fm_vec3_t euler;
    fm_vec3_t target;
    fm_vec3_t velocity;

    fm_vec3_t rot_axis;
    float rot_angle;

    float near_clip = 0.1f;
    float far_clip = 100.0f;
    float FOV = 85.0f;
};

#endif