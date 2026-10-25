#ifndef RESOURCES_H
#define RESOURCES_H

#include "vector"

#include "raylib.h"
#include "raymath.h"


extern const char* lightingVs;
extern const char* lightingFs;

extern Color housingColor;
extern Color chassisColor;
extern Color armColor;
extern Color motorHousingColor;
extern Color rotorShaftColor;
extern Color propColor;

extern std::vector<Vector3> housingVertices;
extern std::vector<Vector3> armVertices;
extern std::vector<Vector3> chassisVertices;
extern std::vector<Vector3> propRightVertices;
extern std::vector<Vector3> propLeftVertices;

#endif