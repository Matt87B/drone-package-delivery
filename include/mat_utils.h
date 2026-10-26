#ifndef MAT_UTILS_H
#define MAT_UTILS_H

#include "raymath.h"

Matrix MatIdentity();
Matrix MatTranslation(float tx, float ty, float tz);
Matrix MatScale(float sx, float sy, float sz);
Matrix MatRotationX(float a);
Matrix MatRotationY(float a);
Matrix MatRotationZ(float a);
Matrix MatMul(Matrix A, Matrix B);
Matrix MatRotation(float rx, float ry, float rz);
Matrix Translate(Vector3 v);
Matrix Scale(Vector3 v);
Matrix Rotation(Vector3 v);
Matrix TRS(const Vector3& t, const Vector3& r, const Vector3& s);
Matrix ParentChild(const Matrix& parentWorld, const Matrix& childLocal);
Vector3 TransformPoint(Vector3 p, Matrix m);
Vector3 GetForwardVector(float yaw);
Vector3 GetRightVector(float yaw);
Vector3 GetUpVector(float pitch, float yaw);
Vector3 ScaleVector(Vector3 v, float factor);
float SmoothStepLerp(float current, float target, float speed);

#endif