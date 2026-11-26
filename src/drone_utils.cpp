#include "drone_utils.h"
#include "resources.h"
#include "mat_utils.h"
#include "raylib.h"

namespace DroneUtils {
    void ToggleActive(Drone& drone) {
        if (drone.isActive) {
            drone.isActive = false;
        }
        else {
            drone.isActive = true;
        }
    }

    void RotateYaw(Drone& drone, float yawInput, float dt) {
        drone.currentYaw += yawInput * YAW_RATE * DEG2RAD * dt;
    }
    void RotatePitch(Drone& drone, float forwardInput, float dt) {
        float pitchSpeed = (forwardInput != 0.0f) ? TILT_SPEED : LEVEL_SPEED;
        float targetPitch = forwardInput * MAX_PITCH_TILT * DEG2RAD;
        drone.currentPitch = SmoothDamp(drone.currentPitch, targetPitch, pitchSpeed * dt);
        drone.currentPitch = Clamp(drone.currentPitch, -MAX_PITCH_TILT, MAX_PITCH_TILT);
    }
    void RotateRoll(Drone& drone, float rightInput, float dt) {
        float rollSpeed  = (rightInput != 0.0f) ? TILT_SPEED : LEVEL_SPEED;
        float targetRoll  = rightInput * MAX_ROLL_TILT * DEG2RAD;
        drone.currentRoll  = SmoothDamp(drone.currentRoll, targetRoll, rollSpeed * dt);
        drone.currentRoll  = Clamp(drone.currentRoll, -MAX_ROLL_TILT, MAX_ROLL_TILT);
    }

    void UpdateOrientation(Drone& drone) {
        Quaternion qDeltaPitch  = QuaternionFromAxisAngle(Vector3{ 1.0f, 0.0f, 0.0f }, drone.currentPitch);
        Quaternion qDeltaRoll   = QuaternionFromAxisAngle(Vector3{ 0.0f, 0.0f, 1.0f }, drone.currentRoll);
        Quaternion qDeltaYaw    = QuaternionFromAxisAngle(Vector3{ 0.0f, 1.0f, 0.0f }, drone.currentYaw);

        drone.orientation = QuaternionMultiply(qDeltaYaw, QuaternionMultiply(qDeltaPitch, qDeltaRoll));
    }

    void UpdateRotation(Drone& drone, float pitchInput, float yawInput, float rollInput, float dt) {
        RotatePitch(drone, pitchInput, dt);
        RotateYaw(drone, yawInput, dt);
        RotateRoll(drone, rollInput, dt);
        UpdateOrientation(drone);
    }

    void UpdateThrottle(Drone& drone, float throttleInput, float dt) {
        
        if (!drone.isActive) {
            drone.throttle = SmoothDamp(drone.throttle, 0.0f, THROTTLE_RATE * dt);
        }
        else {
            if (throttleInput != 0.0f && drone.isActive) {
                drone.throttle += throttleInput * THROTTLE_RATE * dt;
            }
            else {
                drone.throttle = SmoothStepLerp(drone.throttle, HOVER_THROTTLE, THROTTLE_RATE * dt);
            }
        }
        
        drone.throttle = Clamp(drone.throttle, 0.0f, 1.0f);
    }

    void UpdateKinematics(Drone& drone, float dt) {
        Vector3 upVec           = Vector3RotateByQuaternion(Vector3{ 0.0f, 1.0f, 0.0f }, drone.orientation);

        float totalThrust = 0.0f;

        for (int i = 0; i < 4; i++) {
            float speed = drone.rotorShafts[i].angularVelocity;
            totalThrust += MOTOR_THRUST_COEFFICIENT * speed * speed;
        }

        Vector3 thrustForce     = ScaleVector(upVec, totalThrust);
        Vector3 gravityForce    = { 0.0f, -9.81f * DRONE_MASS, 0.0f };
        Vector3 dragForce       = { -drone.velocity.x * HORIZONTAL_DRAG, -drone.velocity.y * VERTICAL_DRAG, -drone.velocity.z * HORIZONTAL_DRAG };
        Vector3 netForce        = Vector3Add(thrustForce, Vector3Add(gravityForce, dragForce));

        drone.acceleration      = ScaleVector(netForce, 1.0f / DRONE_MASS);
        drone.velocity          = Vector3Add(drone.velocity, ScaleVector(drone.acceleration, dt));
        drone.position          = Vector3Add(drone.position, ScaleVector(drone.velocity, dt));
    }

    void UpdateMotorMixer(Drone& drone, float pitchInput, float yawInput, float rollInput) {
        for (RotorShaft& prop : drone.rotorShafts) {
            float motorOutput = drone.throttle
                + pitchInput * prop.pitchSign
                + yawInput * prop.yawSign
                + rollInput * prop.rollSign;
            
            motorOutput = Clamp(motorOutput, 0.0f, 1.0f);
            prop.targetVelocity = motorOutput * MAX_ROTOR_SPEED;
        }
    }

    void UpdatePropellers(Drone& drone, float dt) {
        for (RotorShaft& prop : drone.rotorShafts) {
            prop.angularVelocity = SmoothDamp(prop.angularVelocity, prop.targetVelocity, MOTOR_RESPONSE * dt);
            prop.rotationAngle += prop.spinDirection * prop.angularVelocity * dt;
            prop.rotationAngle = fmodf(prop.rotationAngle, 2.0f * PI);
            prop.rotation.y = prop.rotationAngle * RAD2DEG;
        }
    }

    Drone Init() {
        Drone drone = { 0 };
        drone.position      = dronePosition;
        drone.orientation   = QuaternionIdentity();
        
        drone.chassis.position  = chassisPosition;
        drone.chassis.rotation  = chassisRotation;
        drone.chassis.scale     = chassisScale;
        drone.chassis.color     = chassisColor;

        drone.housing.position  = housingPosition;
        drone.housing.rotation  = housingRotation;
        drone.housing.scale     = housingScale;
        drone.housing.color     = housingColor;

        drone.landingGear.position  = landingGearPosition;
        drone.landingGear.rotation  = landingGearRotation;
        drone.landingGear.scale     = landingGearScale;
        drone.landingGear.color     = landingGearColor;

        for (DronePart arm : drone.arms) {
            arm.color = armColor;
        }
        for (int i = 0; i < std::size(drone.arms); i++) {
            drone.arms[i].position  = armPositions[i];
            drone.arms[i].rotation  = armRotations[i];
            drone.arms[i].scale     = armScale;
        }

        for (DronePart motorHousing : drone.motorHousings) {
            motorHousing.color = motorHousingColor;
        }
        for (int i = 0; i < std::size(drone.motorHousings); i++) {
            drone.motorHousings[i].position = motorHousingPositions[i];
            drone.motorHousings[i].rotation = motorHousingRotations[i];
            drone.motorHousings[i].scale    = motorHousingScale;
        }

        for (DronePart rotorShaft : drone.rotorShafts) {
            rotorShaft.color = rotorShaftColor;
        }
        for (int i = 0; i < std::size(drone.rotorShafts); i++) {
            drone.rotorShafts[i].position   = rotorShaftPositions[i];
            drone.rotorShafts[i].rotation   = rotorShaftRotations[i];
            drone.rotorShafts[i].scale      = rotorShaftScale;
        }
        drone.rotorShafts[0].spinDirection = -1.0f;
        drone.rotorShafts[1].spinDirection = -1.0f;
        drone.rotorShafts[2].spinDirection = 1.0f;
        drone.rotorShafts[3].spinDirection = 1.0f;

        for (DronePart propRight : drone.propRights) {
            propRight.color = propColor;
        }
        for (int i = 0; i < std::size(drone.propRights); i++) {
            drone.propRights[i].position    = propRightPositions[i];
            drone.propRights[i].rotation    = propRightRotations[i];
            drone.propRights[i].scale       = propScale;
        }

        for (DronePart propLeft : drone.propLefts) {
            propLeft.color = propColor;
        }
        for (int i = 0; i < std::size(drone.propLefts); i++) {
            drone.propLefts[i].position = propLeftPositions[i];
            drone.propLefts[i].rotation = propLeftRotations[i];
            drone.propLefts[i].scale    = propScale;
        }

        return drone;
    }
}
