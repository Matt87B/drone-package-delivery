#ifndef DRONE_H
#define DRONE_H

#include "cmath"
#include "raylib.h"
#include "game_object.h"
#include "mat_utils.h"

inline constexpr float TILT_SPEED       = 10.0f;
inline constexpr float LEVEL_SPEED      = 5.0f;

inline constexpr float YAW_RATE         = 90.0f;

inline constexpr float MAX_PITCH_TILT   = 35.0f;
inline constexpr float MAX_ROLL_TILT    = 30.0f;

inline constexpr float DRONE_MASS       = 1.2f;
inline constexpr float GRAVITY          = 9.81f;
inline constexpr float HORIZONTAL_DRAG  = 0.15f;
inline constexpr float VERTICAL_DRAG    = 0.25f;

inline constexpr float THROTTLE_RATE            = 5.0f;
inline constexpr float MAX_ROTOR_SPEED          = 4500.0f;
inline constexpr float HOVER_ROTOR_SPEED        = 2000.0f;
inline constexpr float MOTOR_THRUST_COEFFICIENT = (DRONE_MASS * GRAVITY) / (4.0f * HOVER_ROTOR_SPEED * HOVER_ROTOR_SPEED);
inline constexpr float HOVER_THROTTLE           = sqrtf((DRONE_MASS * 9.81f) / (4.0f * MOTOR_THRUST_COEFFICIENT)) / MAX_ROTOR_SPEED;

inline constexpr float MOTOR_RESPONSE   = 10.0f;
inline constexpr float PITCH_MIX        = 0.20f;
inline constexpr float YAW_MIX          = 0.15f;
inline constexpr float ROLL_MIX         = 0.20f;

typedef struct DroneModels {
    Model housing;
    Model chassis;
    Model landingGear;
    Model arm;
    Model motorHousing;
    Model rotorShaft;
    Model propRight;
    Model propLeft;
} DroneModels;

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

class Drone : public GameObject {
private:
    Vector3 acceleration;
    Quaternion orientation;

    float currentRoll   = 0.0f;
    float currentPitch  = 0.0f;
    float currentYaw    = 0.0f;
    float throttle      = HOVER_THROTTLE;
    bool isActive       = true;

    DronePart chassis;
    DronePart housing;
    DronePart landingGear;
    DronePart arms[4];
    DronePart motorHousings[4];
    RotorShaft rotorShafts[4];
    DronePart propLefts[4];
    DronePart propRights[4];

    void UpdateRotation(float pitchInput, float yawInput, float rollInput, float dt);
    void UpdateThrottle(float throttleInput, float dt);
    void UpdateKinematics(float dt);
    void UpdateMotorMixer(float pitchInput, float yawInput, float rollInput);
    void UpdatePropellers(float dt);
    void InitParts();

public:
    Drone(Vector3 startPos = Vector3{ 0.0f, 5.0f, 0.0f });

    void Update(float pitchInput, float yawInput, float rollInput, float throttleInput, float dt);
    void Draw(DroneModels& models, Shader lighting) const;
    void ToggleActive();
    
    BoundingBox GetBoundingBox() const override;

    void OnCollision(const Vector3& bounceForce) override;

    Quaternion GetOrientation() const;
    bool IsActive() const;

    Vector3& GetPositionRef();
    Vector3& GetVelocityRef();
    Vector3 GetPosition() const;
    Vector3 GetVelocity() const;

};

#endif