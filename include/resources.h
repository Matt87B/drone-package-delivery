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
extern Color landingGearColor;

extern std::vector<Vector3> housingVertices;
extern std::vector<Vector3> armVertices;
extern std::vector<Vector3> chassisVertices;
extern std::vector<Vector3> propRightVertices;
extern std::vector<Vector3> propLeftVertices;
extern std::vector<Vector3> landingGearVertices;

extern std::vector<unsigned short> housingIndices;
extern std::vector<Vector3> housingNormals;
extern std::vector<unsigned short> armIndices;
extern std::vector<Vector3> armNormals;
extern std::vector<unsigned short> chassisIndices;
extern std::vector<Vector3> chassisNormals;
extern std::vector<unsigned short> propRightIndices;
extern std::vector<Vector3> propRightNormals;
extern std::vector<unsigned short> propLeftIndices;
extern std::vector<Vector3> propLeftNormals;
extern std::vector<unsigned short> landingGearIndices;
extern std::vector<Vector3> landingGearNormals;

extern Vector3 droneRotation;
extern Vector3 housingRotation;
extern Vector3 chassisRotation;
extern Vector3 legRotation;
extern Vector3 armRotations[];
extern Vector3 motorHousingRotations[];
extern Vector3 rotorShaftRotations[];
extern Vector3 propRightRotations[];
extern Vector3 propLeftRotations[];

extern Vector3 dronePosition;
extern Vector3 housingPosition;
extern Vector3 chassisPosition;
extern Vector3 legPosition;
extern Vector3 armPositions[];
extern Vector3 motorHousingPositions[];
extern Vector3 rotorShaftPositions[];
extern Vector3 propRightPositions[];
extern Vector3 propLeftPositions[];

extern Vector3 housingScale;
extern Vector3 chassisScale;
extern Vector3 armScale;
extern Vector3 legScale;
extern Vector3 motorHousingScale;
extern Vector3 rotorShaftScale;
extern Vector3 propScale;

#endif