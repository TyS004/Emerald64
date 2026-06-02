#ifndef N64CAMERA_H
#define N64CAMERA_H

#include <t3d/t3d.h>

namespace N64{
    struct N64Camera{
        fm_vec3_t transform {{0.0f, 0.0f, 0.0f}};
        fm_vec3_t target = {{0, 0, 0}};

        float rotation_angle = 0.0f;

        float near_clip = 0.1f;
        float far_clip = 100.0f;
        float FOV = 85.0f;
    };
}

#endif