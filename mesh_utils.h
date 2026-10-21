#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include "raylib.h"
#include "raymath.h"

#include "vector"

Mesh CreateCustomMesh(const std::vector<Vector3>& vertices, const std::vector<Vector3>& normals, const std::vector<unsigned short>& indices);
std::vector<unsigned short> GenerateHardSurfaceIndices(size_t vertexCount);
std::vector<Vector3> CalculateNormals(const std::vector<Vector3>& vertices, const std::vector<unsigned short>& indices);

#endif