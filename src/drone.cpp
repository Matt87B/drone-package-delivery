#include "drone.h"
#include "resources.h"
#include "mat_utils.h"
#include "mesh_utils.h"
#include "raylib.h"

Drone::Drone(Vector3 startPos) : GameObject(startPos, false) {
    velocity        = Vector3Zero();
    acceleration    = Vector3Zero();
    orientation     = QuaternionIdentity();

    currentRoll = 0.0f;
    currentPitch = 0.0f;
    currentYaw = 0.0f;
    throttle = HOVER_THROTTLE;
    isActive = true;

    InitParts();
}

void Drone::ToggleActive() { isActive = !isActive; }

void Drone::InitParts() {
    chassis.position  = chassisPosition;
    chassis.rotation  = chassisRotation;
    chassis.scale     = chassisScale;
    chassis.color     = chassisColor;

    housing.position  = housingPosition;
    housing.rotation  = housingRotation;
    housing.scale     = housingScale;
    housing.color     = housingColor;

    landingGear.position  = landingGearPosition;
    landingGear.rotation  = landingGearRotation;
    landingGear.scale     = landingGearScale;
    landingGear.color     = landingGearColor;

    for (int i = 0; i < 4; i++) {
        arms[i].position    = armPositions[i];
        arms[i].rotation    = armRotations[i];
        arms[i].scale       = armScale;
        arms[i].color       = armColor;
    }


    for (int i = 0; i < 4; i++) {
        motorHousings[i].position   = motorHousingPositions[i];
        motorHousings[i].rotation   = motorHousingRotations[i];
        motorHousings[i].scale      = motorHousingScale;
        motorHousings[i].color      = motorHousingColor;
    }

    for (int i = 0; i < 4; i++) {
        rotorShafts[i].position     = rotorShaftPositions[i];
        rotorShafts[i].rotation     = rotorShaftRotations[i];
        rotorShafts[i].scale        = rotorShaftScale;
        rotorShafts[i].color        = rotorShaftColor;
    }
    rotorShafts[0].spinDirection = -1.0f;
    rotorShafts[1].spinDirection = -1.0f;
    rotorShafts[2].spinDirection = 1.0f;
    rotorShafts[3].spinDirection = 1.0f;

    for (int i = 0; i < 4; i++) {
        propRights[i].position  = propRightPositions[i];
        propRights[i].rotation  = propRightRotations[i];
        propRights[i].scale     = propScale;
        propRights[i].color     = propColor;
    }

    for (int i = 0; i < 4; i++) {
        propLefts[i].position   = propLeftPositions[i];
        propLefts[i].rotation   = propLeftRotations[i];
        propLefts[i].scale      = propScale;
        propLefts[i].color      = propColor;
    }
}

void Drone::UpdateRotation(float pitchInput, float yawInput, float rollInput, float dt) {
        
    currentYaw += yawInput * YAW_RATE * DEG2RAD * dt;

    float targetPitch   = pitchInput * MAX_PITCH_TILT * DEG2RAD;
    float targetRoll    = rollInput * MAX_ROLL_TILT * DEG2RAD;

    float pitchSpeed    = (pitchInput != 0.0f) ? TILT_SPEED : LEVEL_SPEED;
    float rollSpeed     = (rollInput != 0.0f) ? TILT_SPEED : LEVEL_SPEED;

    currentPitch    = SmoothDamp(currentPitch, targetPitch, pitchSpeed * dt);
    currentPitch    = Clamp(currentPitch, -MAX_PITCH_TILT, MAX_PITCH_TILT);

    currentRoll     = SmoothDamp(currentRoll, targetRoll, rollSpeed * dt);
    currentRoll     = Clamp(currentRoll, -MAX_ROLL_TILT, MAX_ROLL_TILT);

    Quaternion qPitch   = QuaternionFromAxisAngle(Vector3{ 1.0f, 0.0f, 0.0f }, currentPitch);
    Quaternion qYaw     = QuaternionFromAxisAngle(Vector3{ 0.0f, 1.0f, 0.0f }, currentYaw);
    Quaternion qRoll    = QuaternionFromAxisAngle(Vector3{ 0.0f, 0.0f, 1.0f }, currentRoll);

    Quaternion qTilt    = QuaternionMultiply(qPitch, qRoll);
    orientation         = QuaternionNormalize(QuaternionMultiply(qYaw, qTilt));
}

void Drone::UpdateThrottle(float throttleInput, float dt) {
    if (!isActive) {
            throttle = SmoothDamp(throttle, 0.0f, THROTTLE_RATE * dt);
    }
    else {
        if (throttleInput != 0.0f && isActive) {
            throttle += throttleInput * THROTTLE_RATE * dt;
        }
        else {
            throttle = SmoothStepLerp(throttle, HOVER_THROTTLE, THROTTLE_RATE * dt);
        }
    }
    throttle = Clamp(throttle, 0.0f, 1.0f);
}

void Drone::UpdateKinematics(float dt) {
    Vector3 upVec = Vector3RotateByQuaternion(Vector3{ 0.0f, 1.0f, 0.0f }, orientation);

    float totalRotorSpeed = throttle * MAX_ROTOR_SPEED;
    float thrustMagnitude = 4.0f * MOTOR_THRUST_COEFFICIENT * (totalRotorSpeed * totalRotorSpeed);

    Vector3 thrustForce     = ScaleVector(upVec, thrustMagnitude);
    Vector3 gravityForce    = Vector3{ 0.0f, -DRONE_MASS * GRAVITY, 0.0f };
    Vector3 dragForce       = {
        -velocity.x * HORIZONTAL_DRAG,
        -velocity.y * VERTICAL_DRAG,
        -velocity.z * HORIZONTAL_DRAG
    };

    Vector3 netForce = Vector3Add(thrustForce, Vector3Add(gravityForce, dragForce));

    acceleration    = ScaleVector(netForce, 1.0f / DRONE_MASS);
    velocity        = Vector3Add(velocity, ScaleVector(acceleration, dt));
    position        = Vector3Add(position, ScaleVector(velocity, dt));
}

void Drone::UpdateMotorMixer(float pitchInput, float yawInput, float rollInput) {
    float baseSpeed = throttle * MAX_ROTOR_SPEED;
    for (int i = 0; i < 4; i++) {
        float mix = (pitchInput * rotorShafts[i].pitchSign * PITCH_MIX) +
                    (yawInput * rotorShafts[i].yawSign * YAW_MIX) +
                    (rollInput * rotorShafts[i].rollSign * ROLL_MIX);
        
        rotorShafts[i].targetVelocity = Clamp(baseSpeed * (1.0f + mix), 0.0f, MAX_ROTOR_SPEED);
    }
}

void Drone::UpdatePropellers(float dt) {
    for (int i = 0; i < 4; i++) {
        rotorShafts[i].angularVelocity  = Lerp(rotorShafts[i].angularVelocity, rotorShafts[i].targetVelocity, MOTOR_RESPONSE * dt);
        rotorShafts[i].rotationAngle    += rotorShafts[i].spinDirection * rotorShafts[i].angularVelocity * dt;
        rotorShafts[i].rotation.y       = rotorShafts[i].rotationAngle;
    }
}

void Drone::Update(float pitchInput, float yawInput, float rollInput, float throttleInput, float dt) {
    Drone::UpdateThrottle(throttleInput, dt);
    Drone::UpdateRotation(pitchInput, yawInput, rollInput, dt);
    Drone::UpdateMotorMixer(pitchInput, yawInput, rollInput);
    Drone::UpdatePropellers(dt);
    Drone::UpdateKinematics(dt);
}

BoundingBox Drone::GetBoundingBox() const {
    Vector3 size = { 1.2f, 0.4f, 1.2f };
    Vector3 halfSize = Vector3Scale(size, 0.5f);
    return BoundingBox{
        Vector3Subtract(position, halfSize),
        Vector3Add(position, halfSize)
    };
}

void Drone::OnCollision(const Vector3& bounceForce) {
    GameObject::OnCollision(bounceForce);
}

void Drone::Draw(DroneModels& models, Shader lighting) const {
    DrawAxes(3.0f, position, Vector3{ currentPitch, currentYaw, currentRoll });

    Matrix worldRoot        = MatrixIdentity();
    Matrix droneWorld       = ParentChild(worldRoot, TRS(position, orientation, Vector3{ 1.0f, 1.0f, 1.0f }));
    Matrix housingWorld     = ParentChild(droneWorld, TRS(housing.position, housing.rotation, housing.scale));
    Matrix chassisWorld     = ParentChild(housingWorld, TRS(chassis.position, chassis.rotation, chassis.scale));
    Matrix landingGearWorld = ParentChild(housingWorld, TRS(landingGear.position, landingGear.rotation, landingGear.scale));

    DrawModelWithWorldMatrix(models.housing, housingWorld, lighting, housing.color);
    DrawModelWithWorldMatrix(models.chassis, chassisWorld, lighting, chassis.color);
    DrawModelWithWorldMatrix(models.landingGear, landingGearWorld, lighting, landingGear.color);

    for (int i = 0; i < 4; i++) {
        Matrix armWorld         = ParentChild(housingWorld, TRS(arms[i].position, arms[i].rotation, arms[i].scale));
        Matrix motorWorld       = ParentChild(armWorld, TRS(motorHousings[i].position, motorHousings[i].rotation, motorHousings[i].scale));
        Matrix shaftWorld       = ParentChild(motorWorld, TRS(rotorShafts[i].position, rotorShafts[i].rotation, rotorShafts[i].scale));
        if (i == 0 || i == 1) {
            for (int j = 0; j < 2; j++) {
                Matrix propRightWorld   = ParentChild(shaftWorld, TRS(propRights[j].position, propRights[j].rotation, propRights[j].scale));
                DrawModelWithWorldMatrix(models.propRight, propRightWorld, lighting, propRights[j].color);
            }
        }
        if (i == 2 || i == 3) {
            for (int j = 0; j < 2; j++) {
                Matrix propLeftWorld    = ParentChild(shaftWorld, TRS(propLefts[j].position, propLefts[j].rotation, propLefts[j].scale));
                DrawModelWithWorldMatrix(models.propLeft, propLeftWorld, lighting, propLefts[j].color);
            }
        }

        DrawModelWithWorldMatrix(models.arm, armWorld, lighting, arms[i].color);
        DrawModelWithWorldMatrix(models.motorHousing, motorWorld, lighting, motorHousings[i].color);
        DrawModelWithWorldMatrix(models.rotorShaft, shaftWorld, lighting, rotorShafts[i].color);
        
    }
}

Vector3& Drone::GetPositionRef() { return position; }
Vector3& Drone::GetVelocityRef() { return velocity; }
Vector3 Drone::GetPosition() const { return position; }
Vector3 Drone::GetVelocity() const { return velocity; }

Quaternion Drone::GetOrientation() const { return orientation; }
bool Drone::IsActive() const { return isActive; }
