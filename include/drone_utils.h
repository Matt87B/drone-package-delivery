#ifndef DRONE_UTILS_H
#define DRONE_UTILS_H

#include "cmath"

#include "raylib.h"

inline constexpr float TILT_SPEED       = 3.0f;
inline constexpr float LEVEL_SPEED      = 5.0f;

inline constexpr float YAW_RATE         = 90.0f;

inline constexpr float MAX_PITCH_TILT   = 35.0f;
inline constexpr float MAX_ROLL_TILT    = 30.0f;

inline constexpr float DRONE_MASS       = 1.2f;
inline constexpr float GRAVITY          = 9.81f;
inline constexpr float HORIZONTAL_DRAG  = 0.25f;
inline constexpr float VERTICAL_DRAG    = 0.7f;

inline constexpr float THROTTLE_RATE            = 4.0f;
inline constexpr float MAX_ROTOR_SPEED          = 1500.0f;
inline constexpr float HOVER_ROTOR_SPEED        = 750.0f;
inline constexpr float MOTOR_THRUST_COEFFICIENT = (DRONE_MASS * GRAVITY) / (4.0f * HOVER_ROTOR_SPEED * HOVER_ROTOR_SPEED);
inline constexpr float HOVER_THROTTLE           = sqrtf((DRONE_MASS * 9.81f) / (4.0f * MOTOR_THRUST_COEFFICIENT)) / MAX_ROTOR_SPEED;

inline constexpr float MOTOR_RESPONSE   = 20.0f;
inline constexpr float PITCH_MIX        = 0.20f;
inline constexpr float YAW_MIX          = 0.15f;
inline constexpr float ROLL_MIX         = 0.20f;

// Custom structs for Drone
typedef struct DronePart {
    Vector3 position; // Position relative to parent position
    Vector3 rotation; // Pitch, Yaw, Roll
    Vector3 scale = Vector3{ 1.0f, 1.0f, 1.0f };

    Color color;
} DronePart;

typedef struct RotorShaft : DronePart {
    float angularVelocity = 0.0f;
    float rotationAngle = 0.0f;

    float targetVelocity = 0.0f;
    float spinDirection = 1.0f;

    float pitchSign = 0.0f;
    float rollSign  = 0.0f;
    float yawSign   = 0.0f;

} RotorShaft;

typedef struct Drone {
    Vector3 position; // World position of drone
    Vector3 velocity;
    Vector3 acceleration;

    float currentRoll    = 0.0f;
    float currentPitch   = 0.0f;
    float currentYaw     = 0.0f;

    float throttle = HOVER_THROTTLE;

    Quaternion orientation;

    bool isActive = true;

    DronePart chassis;
    DronePart housing;
    DronePart landingGear;
    DronePart arms[4];
    DronePart motorHousings[4];
    RotorShaft rotorShafts[4];
    DronePart propLefts[4];
    DronePart propRights[4];

} Drone;

namespace DroneUtils {
    void ToggleActive(Drone& drone);
    void UpdateRotation(Drone& drone, float pitchInput, float yawInput, float rollInput, float dt);
    void UpdateThrottle(Drone& drone, float throttleInput, float dt);
    void UpdateKinematics(Drone& drone, float dt);
    void UpdateMotorMixer(Drone& drone, float pitchInput, float yawInput, float rollInput);
    void UpdatePropellers(Drone& drone, float dt);
    Drone Init();
}


#endif