#ifndef CAM_UTILS_H
#define CAM_UTILS_H

#include "raylib.h"
#include "drone_utils.h"

inline constexpr float CAM_SMOOTH_SPEED = 8.0f;

namespace CamUtils {
    Camera3D Init();
    Camera3D InitOrtho();
    void UpdateCamera(Camera3D& camera, Drone& drone, float dt);
    void UpdateOrthoCamera(Camera3D& camera, Drone& drone, float dt);
}

#endif