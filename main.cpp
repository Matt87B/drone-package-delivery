#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Dimensions
static float housingWidth = 4.0f;
static float housingLength = 6.0f;
static float housingDepth = 3.0f;

// Angles
static float droneAngleDeg = 0.0f;

// Colours
static Color droneHousingColor = Color{ 40, 120,  40, 255 };


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

// Wrappers for matrix operations
static Matrix Translate(Vector3 v) {
    return MatTranslation(v.x, v.y, v.z);
}

static Matrix Scale(Vector3 v) {
    return MatScale(v.x, v.y, v.z);
}

static Matrix Rotation(int axis, float degrees) {
    float a = degrees * DEG2RAD;
    return axis == 0 ? MatRotationX(a) : axis == 1 ? MatRotationY(a) : MatRotationZ(a);
}

// Matrix translate rotate scale
static Matrix TRS(const Vector3& t, int axis, float angleRad, const Vector3& s) {
    Matrix T = Translate(t);
    Matrix R = Rotation(axis, angleRad * RAD2DEG);
    Matrix S = Scale(s);
    return MatrixMultiply(MatrixMultiply(S, R), T);
}

// Hierarchical matrix operation
static Matrix ParentChild(const Matrix& parentWorld, const Matrix& childLocal) {
    return MatrixMultiply(childLocal, parentWorld);
}

// Draw Axes
static void DrawAxes(float len) {
    DrawLine3D({0, 0, 0}, {len, 0, 0}, RED);
    DrawLine3D({0, 0, 0}, {0, len, 0}, GREEN);
    DrawLine3D({0, 0, 0}, {0, 0, len}, BLUE);
    DrawSphere({0, 0, 0}, 0.09f, YELLOW);
}

static Vector3 TransformPoint(Vector3 p, Matrix m) {
    return Vector3Transform(p, m);
}

// Draw model with world matrix
static void DrawModelWithWorldMatrix(Model& model, const Matrix& world, Color tint = WHITE) {
    Matrix prev = model.transform;
    model.transform = world;

    float color[4] =
    {
        tint.r / 255.0f,
        tint.g / 255.0f,
        tint.b / 255.0f,
        tint.a / 255.0f
    };

    DrawModel(model, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, BLACK);

    model.transform = prev;
}


// Main
int main(void) {

    InitWindow(800, 450, "raylib - drone model");

    Camera3D camera = { 0 };
    camera.position = (Vector3) { 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // Models

    Model cubeModel  = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    RenderTexture2D target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose()) {
        camera.target = 

        if (IsKeyDown(KEY_W)) {
            
        }

        if (IsKeyDown(KEY_A)) {
            
        }

        if (IsKeyDown(KEY_S)) {
            
        }

        if (IsKeyDown(KEY_D)) {
            
        }

        if (IsKeyDown(KEY_C) || IsKeyDown(KEY_LEFT_CONTROL)) {
            
        }

        if (IsKeyDown(KEY_SPACE)) {
            
        }


        // Hierarchical matrices

        Matrix worldRoot = MatrixIdentity();

        Matrix droneWorld =
            ParentChild(
                worldRoot,
                TRS(
                    Vector3{ 2.0f, 2.0f, 2.0f },
                    1,
                    droneAngleDeg,
                    Vector3{ 1.0f, 1.0f, 1.0f }
                )
            );


        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                
                // Draw all 3D geometry here
                DrawModelWithWorldMatrix(cubeModel, droneWorld);
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
