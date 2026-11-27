#include "cam_utils.h"

#include "raylib.h"
#include "mat_utils.h"
#include "raymath.h"
#include "drone.h"

namespace CamUtils {
    Camera3D Init() {
        Camera3D camera     = { 0 };
        camera.position     = (Vector3) { 0.0f, 10.0f, -10.0f };
        camera.target       = (Vector3) { 0.0f, 0.0f, 0.0f };
        camera.up           = (Vector3) { 0.0f, 1.0f, 0.0f };
        camera.fovy         = 45.0f;
        camera.projection   = CAMERA_PERSPECTIVE;

        return camera;
    }

    Camera3D InitOrtho() {
        Camera3D camera     = { 0 };
        camera.position     = (Vector3) { 0.0f, 10.0f, 0.0f };
        camera.target       = (Vector3) { 0.0f, 0.0f, 0.0f };
        camera.up           = (Vector3) {0.0f, 0.0f, -1.0f };
        camera.fovy         = 15.0f;
        camera.projection   = CAMERA_ORTHOGRAPHIC;

        return camera;
    }

    void UpdateCamera(Camera3D& camera, Drone& drone, float dt) {
        Vector3 localCamOffset  = { 0.0f, 4.0f, -10.0f };
        Vector3 rotatedOffset   = Vector3RotateByQuaternion(localCamOffset, drone.GetOrientation());
        Vector3 targetCamPos    = Vector3Add(drone.GetPosition(), rotatedOffset);

        camera.position     = Vector3Lerp(camera.position, targetCamPos, Clamp(CAM_SMOOTH_SPEED * dt, 0.0f, 1.0f));
        camera.target       = drone.GetPosition();
        camera.up           = Vector3RotateByQuaternion(Vector3{ 0.0f, 1.0f, 0.0f }, drone.GetOrientation());
    }

    void UpdateOrthoCamera(Camera3D& camera, Drone& drone, float dt) {
        Vector3 bellyOffset  = Vector3RotateByQuaternion({ 0.0f, -1.0f, 0.0f }, drone.GetOrientation());
        Vector3 lookOffset   = Vector3RotateByQuaternion({ 0.0f, -10.0f, 0.0f }, drone.GetOrientation());
        Vector3 forwardDir   = Vector3RotateByQuaternion({ 0.0f, 0.0f, -1.0f }, drone.GetOrientation());

        camera.position = Vector3Add(drone.GetPosition(), bellyOffset);
        camera.target   = Vector3Add(drone.GetPosition(), lookOffset);
        camera.up       = forwardDir;
    }

}