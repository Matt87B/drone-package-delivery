#include "mat_utils.h"
#include "raymath.h"

// Matrix transformation helpers
Matrix MatIdentity() {
    return MatrixIdentity();
}

Matrix MatTranslation(float tx, float ty, float tz) {
    Matrix m = MatrixIdentity();
    m.m12 = tx;
    m.m13 = ty;
    m.m14 = tz;
    return m;
}

Matrix MatScale(float sx, float sy, float sz) {
    Matrix m = MatrixIdentity();
    m.m0 = sx;
    m.m5 = sy;
    m.m10 = sz;
    return m;
}

Matrix MatRotationX(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m5 = c;
    m.m9 = -s;
    m.m6 = s;
    m.m10 = c;
    return m;
}

Matrix MatRotationY(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m8 = s;
    m.m2 = -s;
    m.m10 = c;
    return m;
}

Matrix MatRotationZ(float a) {
    float c = cosf(a), s = sinf(a);
    Matrix m = MatrixIdentity();
    m.m0 = c;
    m.m4 = -s;
    m.m1 = s;
    m.m5 = c;
    return m;
}

Matrix MatMul(Matrix A, Matrix B) {
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

// Rotate about three axes
Matrix MatRotation(float rx, float ry, float rz) {
    Matrix matX = MatRotationX(rx * DEG2RAD);
    Matrix matY = MatRotationY(ry * DEG2RAD);
    Matrix matZ = MatRotationZ(rz * DEG2RAD);

    return MatMul(MatMul(matX, matY), matZ);
}

// Wrappers for matrix operations
Matrix Translate(Vector3 v) {
    return MatTranslation(v.x, v.y, v.z);
}

Matrix Scale(Vector3 v) {
    return MatScale(v.x, v.y, v.z);
}

Matrix Rotation(Vector3 v) {
    return MatRotation(v.x, v.y, v.z);
}

// Matrix translate rotate scale
Matrix TRS(const Vector3& t, const Vector3& r, const Vector3& s) {
    Matrix T = Translate(t);
    Matrix R = Rotation(r);
    Matrix S = Scale(s);
    return MatrixMultiply(MatrixMultiply(S, R), T);
}

// Assign a child matrix to a world
Matrix ParentChild(const Matrix& parentWorld, const Matrix& childLocal) {
    return MatrixMultiply(childLocal, parentWorld);
}

// Transform a vector in accordance with a given transformation matrix
Vector3 TransformPoint(Vector3 p, Matrix m) {
    return Vector3Transform(p, m);
}

Vector3 GetForwardVector(float yaw) {
    Vector3 forward;
    forward.x = sinf(yaw);
    forward.y = 0.0f;
    forward.z = cosf(yaw);
    return forward;
}

Vector3 GetRightVector(float yaw) {
    Vector3 right;
    right.x = cosf(yaw);
    right.y = 0.0f;
    right.z = -sinf(yaw);
    return right;
}

Vector3 GetUpVector(float pitch, float yaw) {
    Vector3 up;
    up.x = sinf(pitch) * sinf(yaw);
    up.y = cosf(pitch);
    up.z = sinf(pitch) * cosf(yaw);
    return up;
}

Vector3 ScaleVector(Vector3 v, float factor) {
    return Vector3 { v.x * factor, v.y * factor, v.z * factor };
}