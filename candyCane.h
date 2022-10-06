#ifndef CANDY_CANE
#define CANDY_CANE

#include "CSCIx229.h"

void Circle(float circlePrecision, float r, float ox, float oy, float oz);
void RedStripedCylinderWall(int circlePrecision, float crossRad, float straightHeight);
void RedStripedHookSegment(int circlePrecision, float crossRad, float hookRad);
void CandyCane(float crossRad, float straightHeight, float hookRad, int hookDeg);

#endif

