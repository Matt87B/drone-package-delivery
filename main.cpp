#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Dimensions
static float housingWidth = 4.0f;
static float housingLength = 6.0f;
static float housingDepth = 3.0f;

// Colours
static Color housingColor = Color{ 40, 120,  40, 255 };
static Color chassisColor = Color{ 30, 100, 70, 255 };

// Angles
Vector3 droneRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 housingRotation = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisRotation = Vector3{ 0.0f, 0.0f, 0.0f };

// Dynamic positions
Vector3 dronePosition   = Vector3{ 0.0f, 3.0f, 0.0f };
Vector3 housingPosition = Vector3{ 0.0f, 0.0f, 0.0f };
Vector3 chassisPosition = Vector3{ 0.0f, 0.0f, 0.0f };

Vector3 housingScale    = Vector3{ 1.0f, 1.0f, 1.0f };
Vector3 chassisScale    = Vector3{ 1.0f, 1.0f, 1.0f };

typedef struct DronePart {
    Vector3 position; // Position relative to parent position
    Vector3 rotation; // Pitch, Yaw, Roll
    Vector3 scale;

    Color color;
} DronePart;

typedef struct Drone {
    Vector3 position; // World position of chassis
    Vector3 rotation; // World rotation of chassis

    DronePart chassis;
    DronePart housing;
    //DronePart propellers[4];
} Drone;


// Matrix transformation helpers

static Matrix MatIdentity() {
    return MatrixIdentity();
}

static Matrix MatTranslation(float tx, float ty, float tz) {
    Matrix m = MatrixIdentity();
    m.m12 = tx;
    m.m13 = ty;
    m.m14 = tz;
    return m;
}

static Matrix MatScale(float sx, float sy, float sz) {
    Matrix m = MatrixIdentity();
    m.m0 = sx;
    m.m5 = sy;
    m.m10 = sz;
    return m;
}

static Matrix MatRotationX(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m5 = c;
    m.m9 = -s;
    m.m6 = s;
    m.m10 = c;
    return m;
}

static Matrix MatRotationY(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m8 = s;
    m.m2 = -s;
    m.m10 = c;
    return m;
}

static Matrix MatRotationZ(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m4 = -s;
    m.m1 = s;
    m.m5 = c;
    return m;
}

static Matrix MatMul(Matrix A, Matrix B) {
    float a[4][4] = {{A.m0, A.m4, A.m8, A.m12},
                     {A.m1, A.m5, A.m9, A.m13},
                     {A.m2, A.m6, A.m10, A.m14},
                     {A.m3, A.m7, A.m11, A.m15}};
    float b[4][4] = {{B.m0, B.m4, B.m8, B.m12},
                     {B.m1, B.m5, B.m9, B.m13},
                     {B.m2, B.m6, B.m10, B.m14},
                     {B.m3, B.m7, B.m11, B.m15}};
    float c[4][4] = {};
    for (int r = 0; r < 4; ++r)
    {
        for (int col = 0; col < 4; ++col)
        {
            for (int k = 0; k < 4; ++k)
            {
                c[r][col] += a[r][k] * b[k][col];
            }
        }
    }

    Matrix M = {};
    M.m0 = c[0][0];
    M.m4 = c[0][1];
    M.m8 = c[0][2];
    M.m12 = c[0][3];
    M.m1 = c[1][0];
    M.m5 = c[1][1];
    M.m9 = c[1][2];
    M.m13 = c[1][3];
    M.m2 = c[2][0];
    M.m6 = c[2][1];
    M.m10 = c[2][2];
    M.m14 = c[2][3];
    M.m3 = c[3][0];
    M.m7 = c[3][1];
    M.m11 = c[3][2];
    M.m15 = c[3][3];
    return M;
}

// Rotate about all three axes
static Matrix MatRotation(float rx, float ry, float rz) {
    Matrix matX = MatRotationX(rx);
    Matrix matY = MatRotationY(ry);
    Matrix matZ = MatRotationZ(rz);

    return MatMul(MatMul(matX, matY), matZ);
}

// Wrappers for matrix operations
static Matrix Translate(Vector3 v) {
    return MatTranslation(v.x, v.y, v.z);
}

static Matrix Scale(Vector3 v) {
    return MatScale(v.x, v.y, v.z);
}

static Matrix Rotation(Vector3 v) {
    return MatRotation(v.x, v.y, v.z);
}

// Matrix translate rotate scale
static Matrix TRS(const Vector3& t, const Vector3& r, const Vector3& s) {
    Matrix T = Translate(t);
    Matrix R = Rotation(r);
    Matrix S = Scale(s);
    return MatrixMultiply(MatrixMultiply(T, R), S);
}

// Assign a child matrix to a world
static Matrix ParentChild(const Matrix& parentWorld, const Matrix& childLocal) {
    return MatrixMultiply(parentWorld, childLocal);
}

static void DrawAxes(float len) {
    DrawLine3D({0, 0, 0}, {len, 0, 0}, RED);
    DrawLine3D({0, 0, 0}, {0, len, 0}, GREEN);
    DrawLine3D({0, 0, 0}, {0, 0, len}, BLUE);
    DrawSphere({0, 0, 0}, 0.09f, YELLOW);
}

// Transform a vector in accordance with a given transformation matrix
static Vector3 TransformPoint(Vector3 p, Matrix m) {
    return Vector3Transform(p, m);
}

// Draw model relative to a world matrix
static void DrawModelWithWorldMatrix(Model& model, const Matrix& world, Color color) {
    Matrix prev = model.transform;
    model.transform = world;

    DrawModel(model, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, color);

    model.transform = prev;
}

// Initialize Drone struct
static Drone InitDrone() {
    Drone drone = { 0 };
    drone.position = dronePosition;
    drone.rotation = droneRotation;
    
    drone.chassis.position = chassisPosition;
    drone.chassis.rotation = chassisRotation;
    drone.chassis.scale = chassisScale;
    drone.chassis.color = chassisColor;

    drone.housing.position = housingPosition;
    drone.housing.rotation = housingRotation;
    drone.housing.scale = housingScale;
    drone.housing.color = housingColor;

    return drone;
}

// Initialize Camera3D
Camera3D InitCamera() {
    Camera3D camera = { 0 };
    camera.position = (Vector3) { 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    return camera;
}

// Main loop
int main(void) {

    InitWindow(800, 450, "raylib - drone model");

    Camera3D camera = InitCamera();
    Drone drone = InitDrone();

    SetTargetFPS(60);

    // Models

    Model droneHousingModel  = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
    Model droneChassisModel = LoadModelFromMesh(GenMeshCube(3.0f, 1.0f, 1.0f));

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose()) {

        // Key listeners

        if (IsKeyDown(KEY_A)) {
            drone.chassis.position.x -= 0.1f;
        }
        if (IsKeyDown(KEY_D)) {
            drone.chassis.position.x += 0.1f;
        }
        if (IsKeyDown(KEY_S)) {
            drone.position.z -= 0.1f;
        }
        if (IsKeyDown(KEY_W)) {
            drone.position.z += 0.1f;
        }
        if (IsKeyDown(KEY_C) || IsKeyDown(KEY_LEFT_CONTROL)) {
            drone.position.y -= 0.1f;
        }
        if (IsKeyDown(KEY_SPACE)) {
            drone.position.y += 0.1f;
        }

        // Hierarchical matrices

        // World root
        Matrix worldRoot = MatrixIdentity();

        // Child of world root
        Matrix droneWorld =
            ParentChild(
                worldRoot,
                TRS(
                    drone.position,
                    drone.rotation,
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
        Matrix chassisWorld =
            ParentChild(
                droneWorld,
                TRS(
                    drone.chassis.position,
                    drone.chassis.rotation,
                    drone.chassis.scale
                )
            );


        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                // Draw all 3D geometry here
                DrawModelWithWorldMatrix(droneHousingModel, housingWorld, housingColor);
                DrawModelWithWorldMatrix(droneChassisModel, chassisWorld, chassisColor);

                DrawGrid(100, 1.0f);
                DrawAxes(3.0f);
            EndMode3D();

            // Draw all 2D geometry here
            DrawFPS(10, 40);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
