/*
 * Author: John Salame
 * Contains the definitions for objects used by the scenes (or by the main file)
 */
#include "myCSCI5229.h"

#ifndef MY_OBJECTS
#define MY_OBJECTS

// threeDCos is a function to create a mound of snow (snow.c)
void threeDCos(unsigned int texName, unsigned int normalMapName, unsigned int programName);
// functions related to the candy cane (candyCane.c)
void Circle(float circlePrecision, float r, float ox, float oy, float oz, unsigned int programName);
void RedStripedCylinderWall(int circlePrecision, float crossRad, float straightHeight, unsigned int programName);
void RedStripedHookSegment(int circlePrecision, float crossRad, float hookRad);
void CandyCane(float crossRad, float straightHeight, float hookRad, int hookDeg, unsigned int texName, unsigned int normalMapName, unsigned int programName);
// ball for showing the position of lights
void ball(double x,double y,double z,double r);
void Vertex(double th,double ph);
// bench.c
void createFillet(float right, float radius, float side, float top, unsigned int programName, float texS1, float texS2);
void buttSlat(float thickness, float filletRad, float flatWidth, unsigned int programName, float texS1, float texS2);
void bench(float width, float height, float depth, unsigned int texName, unsigned int normalMapName, unsigned int programName);

#endif

