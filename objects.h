/*
 * Author: John Salame
 * Contains the definitions for objects used by the scenes (or by the main file)
 */
#include "myCSCI5229.h"

#ifndef MY_OBJECTS
#define MY_OBJECTS

// threeDCos is a function to create a mound of snow (snow.c)
void threeDCos(unsigned int texName);
// functions related to the candy cane (candyCane.c)
void Circle(float circlePrecision, float r, float ox, float oy, float oz);
void RedStripedCylinderWall(int circlePrecision, float crossRad, float straightHeight);
void RedStripedHookSegment(int circlePrecision, float crossRad, float hookRad);
void CandyCane(float crossRad, float straightHeight, float hookRad, int hookDeg, unsigned int texName);
// ball for showing the position of lights
void ball(double x,double y,double z,double r);
void Vertex(double th,double ph);

#endif

