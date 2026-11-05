// C++ imports
#include "vector"

// Raylib imports
#include "raylib.h"
#include "raymath.h"

// Workspace imports
#include "mesh_utils.h"
#include "mat_utils.h"
#include "drone_utils.h"
#include "resources.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define DRONE_SPEED 5.0f
#define SLANT_OFFSET 10.0f
#define SLANT_SPEED 2.0f
#define ROTOR_SHAFT_SPEED 1500.0f
#define MAX_TILT 40.0f 

#define DRONE_MASS 1.2f
#define MAX_THRUST 35.0f
#define LINEAR_DRAG 1.2f
#define YAW_TORQUE 15.0f
#define YAW_DRAG 5.0f
#define ANGULAR_DAMPENING 10.0f
#define CLIMB_THRUST_POWER 16.0f
#define PITCH_RATE 120.0f
#define YAW_RATE 90.0f
#define ROLL_RATE 120.0f
#define CAM_SMOOTH_SPEED 8.0f


// Lighting uniforms
Vector3 lightPos = { 20.0f, 20.0f, 20.0f };

float ambientStrength = 0.4f;
float specularStrength = 0.50f;
float shininess = 200.0f;

// Init functions

// Initialize Camera3D
Camera3D InitCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3) { 0.0f, 10.0f, -10.0f };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}

static void DrawAxes(float len, Matrix transform) {
    Vector3 origin = TransformPoint(Vector3{ 0.0f, 0.0f, 0.0f }, transform);
    Vector3 posX   = TransformPoint(Vector3{ len, 0.0f, 0.0f }, transform);
    Vector3 posY   = TransformPoint(Vector3{ 0.0f, len, 0.0f }, transform);
    Vector3 posZ   = TransformPoint(Vector3{ 0.0f, 0.0f, len }, transform);

    DrawLine3D(origin, posX, RED);   // Local X axis (Red)
    DrawLine3D(origin, posY, GREEN); // Local Y axis (Green)
    DrawLine3D(origin, posZ, BLUE);  // Local Z axis (Blue)
    DrawSphere(origin, 0.09f, YELLOW);
}

static void DrawAxes(float len, Vector3 pos, Vector3 rot) {
    Matrix transform = TRS(pos, rot, Vector3{ 1.0f, 1.0f, 1.0f });
    DrawAxes(len, transform);
}

// Main loop
int main(void) {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(800, 450, "raylib - drone model");

    // Objects
    Camera3D camera = InitCamera();
    Drone drone = InitDrone();

    SetTargetFPS(60);

    // Meshes
    Mesh housingMesh   = CreateCustomMesh(housingVertices, housingNormals, housingIndices);
    Mesh chassisMesh   = CreateCustomMesh(chassisVertices, chassisNormals, chassisIndices);
    Mesh armMesh       = CreateCustomMesh(armVertices, armNormals, armIndices);
    Mesh motorHousingMesh = GenMeshCylinder(0.25f, 0.4f, 16);
    Mesh landingGearMesh  = CreateCustomMesh(landingGearVertices, landingGearNormals, landingGearIndices);
    Mesh rotorShaftMesh = GenMeshCube(0.2f, 0.4f, 0.2f);
    Mesh propRightMesh = CreateCustomMesh(propRightVertices, propRightNormals, propRightIndices);
    Mesh propLeftMesh = CreateCustomMesh(propLeftVertices, propLeftNormals, propLeftIndices);

    // Models
    Model housingModel = LoadModelFromMesh(housingMesh);
    Model chassisModel = LoadModelFromMesh(chassisMesh);
    Model landingGearModel = LoadModelFromMesh(landingGearMesh);
    Model armModel     = LoadModelFromMesh(armMesh);
    Model motorHousingModel = LoadModelFromMesh(motorHousingMesh);
    Model rotorShaftModel    = LoadModelFromMesh(rotorShaftMesh);
    Model propRightModel = LoadModelFromMesh(propRightMesh);
    Model propLeftModel = LoadModelFromMesh(propLeftMesh);

    // Shader
    Shader lighting = LoadShaderFromMemory(lightingVs, lightingFs);

    housingModel.materials[0].shader   = lighting;
    chassisModel.materials[0].shader   = lighting;
    landingGearModel.materials[0].shader = lighting;
    armModel.materials[0].shader       = lighting;
    motorHousingModel.materials[0].shader = lighting;
    rotorShaftModel.materials[0].shader   = lighting;
    propRightModel.materials[0].shader = lighting;
    propLeftModel.materials[0].shader = lighting;

    int baseColorLoc = GetShaderLocation(lighting, "baseColor");

    // Lighting

    int lightPosLoc = GetShaderLocation(lighting, "lightPos");
    int viewPosLoc = GetShaderLocation(lighting, "viewPos");
    int ambientLoc = GetShaderLocation(lighting, "ambientStrength");
    int specularLoc = GetShaderLocation(lighting, "specularStrength");
    int shininessLoc = GetShaderLocation(lighting, "shininess");

    // Game loop
    while (!WindowShouldClose()) {
        
        float dt = GetFrameTime();

        if (drone.isActive) {

            float forwardInput  = 0.0f;
            float rightInput    = 0.0f;
            float climbInput    = 0.0f;
            float yawInput      = 0.0f;

            // Input polling
            if (IsKeyDown(KEY_W)) forwardInput  += 1.0f;
            if (IsKeyDown(KEY_S)) forwardInput  -= 1.0f;
            if (IsKeyDown(KEY_D)) rightInput    += 1.0f;
            if (IsKeyDown(KEY_A)) rightInput    -= 1.0f;
            if (IsKeyDown(KEY_R)) climbInput    += 1.0f;
            if (IsKeyDown(KEY_F)) climbInput    -= 1.0f;
            if (IsKeyDown(KEY_Q)) yawInput      += 1.0f;
            if (IsKeyDown(KEY_E)) yawInput      -= 1.0f;

            // Rotation logic
            float pitchAngleRad     = forwardInput * PITCH_RATE * DEG2RAD * dt;
            float yawAngleRad       = yawInput * YAW_RATE * DEG2RAD * dt;
            float rollAngleRad      = rightInput * ROLL_RATE * DEG2RAD * dt;


            Quaternion qDeltaPitch  = QuaternionFromAxisAngle(Vector3{ 1.0f, 0.0f, 0.0f }, pitchAngleRad);
            Quaternion qDeltaRoll   = QuaternionFromAxisAngle(Vector3{ 0.0f, 0.0f, 1.0f }, rollAngleRad);
            Quaternion qDeltaYaw    = QuaternionFromAxisAngle(Vector3{ 0.0f, 1.0f, 0.0f }, yawAngleRad);

            Quaternion qDelta = QuaternionMultiply(qDeltaYaw, QuaternionMultiply(qDeltaPitch, qDeltaRoll));
            
            if (Vector3Length(Vector3{ drone.orientation.x, drone.orientation.y, drone.orientation.z }) == 0.0f && drone.orientation.w == 0.0f) {
                drone.orientation = QuaternionIdentity();
            }

            drone.orientation = QuaternionNormalize(QuaternionMultiply(drone.orientation, qDelta));

            Matrix bodyRotation = QuaternionToMatrix(drone.orientation);
            Vector3 upVec       = Vector3RotateByQuaternion(Vector3{ 0.0f, 1.0f, 0.0f }, drone.orientation);

            // Thrust physics
            //float totalTiltDeg = sqrtf(drone.rotation.x * drone.rotation.x + drone.rotation.z * drone.rotation.z);
            //float tiltCompensation  = Clamp(1.0f / cosf(totalTiltDeg * DEG2RAD), 1.0f, 1.5f);
            Vector3 gravityForce    = { 0.0f, -9.81f * DRONE_MASS, 0.0f };

            float hoverThrust       = 9.81f * DRONE_MASS; //* tiltCompensation;
            Vector3 hoverForce      = ScaleVector(upVec, hoverThrust);
            Vector3 climbForce      = ScaleVector(upVec, climbInput * CLIMB_THRUST_POWER);

            Vector3 thrustForce     = Vector3Add(hoverForce, climbForce);
            Vector3 dragForce       = ScaleVector(drone.velocity, -LINEAR_DRAG);
            Vector3 netForce        = Vector3Add(thrustForce, Vector3Add(gravityForce, dragForce));

            drone.acceleration  = ScaleVector(netForce, 1.0f / DRONE_MASS);
            drone.velocity      = Vector3Add(drone.velocity, ScaleVector(drone.acceleration, dt));
            drone.position      = Vector3Add(drone.position, ScaleVector(drone.velocity, dt));


            // Propeller spin animation
            for (int i = 0; i < 2; i++) {drone.rotorShafts[i].rotation.y -= ROTOR_SHAFT_SPEED * dt;}
            for (int i = 2; i < 4; i++) {drone.rotorShafts[i].rotation.y += ROTOR_SHAFT_SPEED * dt;}

            // Camera follow logic
            Vector3 localCamOffset = { 0.0f, 4.0f, -10.0f };
            Vector3 rotatedOffset = Vector3RotateByQuaternion(localCamOffset, drone.orientation);
            Vector3 targetCamPos = Vector3Add(drone.position, rotatedOffset);

            camera.position = Vector3Lerp(camera.position, targetCamPos, Clamp(CAM_SMOOTH_SPEED * dt, 0.0f, 1.0f));
            camera.target = drone.position;
            camera.up = Vector3RotateByQuaternion(Vector3{ 0.0f, 1.0f, 0.0f }, drone.orientation);
        }

        // Toggle drone active
        if (IsKeyPressed(KEY_SPACE)) ToggleActive(drone);

        if (IsKeyPressed(KEY_HOME)) drone = InitDrone();

        // Init shader uniforms
        SetShaderValue(lighting, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, viewPosLoc, &camera.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(lighting, ambientLoc, &ambientStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, specularLoc, &specularStrength, SHADER_UNIFORM_FLOAT);
        SetShaderValue(lighting, shininessLoc, &shininess, SHADER_UNIFORM_FLOAT);


        // Hierarchical matrices

        // World root
        Matrix worldRoot = MatrixIdentity();

        // Child of world root
        Matrix droneWorld =
            ParentChild(
                worldRoot,
                TRS(
                    drone.position,
                    drone.orientation,
                    Vector3{ 1.0f, 1.0f, 1.0f } //Drone world's scale is always 1
                )
            );

        // Childs of drone world
        Matrix housingWorld =
            ParentChild(
                droneWorld,
                TRS(
                    drone.housing.position,
                    drone.housing.rotation,
                    drone.housing.scale
                )
            );
        
        Matrix landingGearWorld =
            ParentChild(
                housingWorld,
                TRS(
                    drone.landingGear.position,
                    drone.landingGear.rotation,
                    drone.landingGear.scale
                )
            );

        Matrix chassisWorld =
            ParentChild(
                housingWorld,
                TRS(
                    drone.chassis.position,
                    drone.chassis.rotation,
                    drone.chassis.scale
                )
            );

        Matrix armWorlds[std::size(drone.arms)];
        for (int i = 0; i < std::size(drone.arms); i++) {
            armWorlds[i] = 
                ParentChild(
                housingWorld,
                TRS(
                    drone.arms[i].position,
                    drone.arms[i].rotation,
                    drone.arms[i].scale
                )
            );
        }

        Matrix motorHousingWorlds[std::size(drone.motorHousings)];
        for (int i = 0; i < std::size(drone.motorHousings); i++) {
            motorHousingWorlds[i] = 
                ParentChild(
                armWorlds[i],
                TRS(
                    drone.motorHousings[i].position,
                    drone.motorHousings[i].rotation,
                    drone.motorHousings[i].scale
                )
            );
        }

        Matrix rotorShaftWorlds[std::size(drone.rotorShafts)];
        for (int i = 0; i < std::size(drone.rotorShafts); i++) {
            rotorShaftWorlds[i] =
                ParentChild(
                motorHousingWorlds[i],
                TRS(
                    drone.rotorShafts[i].position,
                    drone.rotorShafts[i].rotation,
                    drone.rotorShafts[i].scale
                )
            );
        }

        Matrix propRightWorlds[std::size(drone.propRights)];
        for (int i = 0; i < std::size(drone.propRights); i++) {
            propRightWorlds[i] = 
                ParentChild(
                rotorShaftWorlds[(i % 2) + 2],
                TRS(
                    drone.propRights[i].position,
                    drone.propRights[i].rotation,
                    drone.propRights[i].scale
                )
            );
        }

        Matrix propLeftWorlds[std::size(drone.propLefts)];
        for (int i = 0; i < std::size(drone.propLefts); i++) {
            propLeftWorlds[i] = 
                ParentChild(
                rotorShaftWorlds[i % 2],
                TRS(
                    drone.propLefts[i].position,
                    drone.propLefts[i].rotation,
                    drone.propLefts[i].scale
                )
            );
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                
                // Debugging for drone local orientation
                DrawAxes(3.0f, drone.position, drone.chassis.rotation);

                // Draw all 3D geometry
                DrawModelWithWorldMatrix(housingModel, housingWorld, lighting, housingColor);
                DrawModelWithWorldMatrix(chassisModel, chassisWorld, lighting, chassisColor);
                DrawModelWithWorldMatrix(landingGearModel, landingGearWorld, lighting, landingGearColor);
                
                for (Matrix armWorld : armWorlds) DrawModelWithWorldMatrix(armModel, armWorld, lighting, armColor);
                for (Matrix motorHousingWorld : motorHousingWorlds) DrawModelWithWorldMatrix(motorHousingModel, motorHousingWorld, lighting, motorHousingColor);
                for (Matrix rotorShaftWorld: rotorShaftWorlds) DrawModelWithWorldMatrix(rotorShaftModel, rotorShaftWorld, lighting, rotorShaftColor);
                for (Matrix propRightWorld: propRightWorlds) DrawModelWithWorldMatrix(propRightModel, propRightWorld, lighting, propColor);
                for (Matrix propLeftWorld: propLeftWorlds) DrawModelWithWorldMatrix(propLeftModel, propLeftWorld, lighting, propColor);

                DrawGrid(100, 1.0f);
                DrawSphere(lightPos, 0.5f, YELLOW);
                
            EndMode3D();

            // Draw all 2D geometry here
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
