#ifndef CAM_UTILS_H
#define CAM_UTILS_H

#include "raylib.h"
#include "drone_utils.h"

inline constexpr float CAM_SMOOTH_SPEED = 8.0f;

namespace CamUtils {
    Camera3D Init();
    void UpdateCamera(Camera3D& camera, float dt, Drone& drone);
}

#endif