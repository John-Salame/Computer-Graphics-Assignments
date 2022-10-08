/*
 * Author: John Salame
 * A file which contains object definitions for snow piles and (in the future) snowflakes
 */
#include "objects.h"

/*
 * The positive part of a 3D Cosine wave centered at (0, 0)
 * Equation: y = 0.5*Cos(180*x)+0.5*Cos(180*z)-0.5, domain x e [-0.5, 0.5], z e [-0.5, 0.5]
 * Good for making mounds of snow
 */
void threeDCos() {
  const double min = -0.5;
  // we do not need a variable for it, but the max is 0.5
  const int interval = 15; // angular interval
  const double cartInterval = (double) interval / 180; // cartesian interval (space between points)
  double xLoc = min;
  double zLoc = min;
  // apply lighting
  float white[] = {1.0, 1.0, 1.0, 1.0}; // Note: The lighting will be such that the snow appears gray at night (lighting enabled = night)
  float blue[] =  {0.1, 0.2, 0.5, 0};
  glColor4fv(white); // without the color, the snow mound is white. It seems like the materials don't do anything at all to the color.
  glMaterialfv(GL_FRONT, GL_AMBIENT, blue); // snow looks blue at night
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
  // b is the z position
  for(int b = -90; b < 90; b+= interval) {
    xLoc = min;
    glBegin(GL_QUAD_STRIP);
    // a is the x position
    for(int a = -90; a <= 90; a += interval) {
      // the normal is the gradient of the function, where the function value is y
      glNormal3f(0.5*3.14159*Sin(a), 1.0, 0.5*3.14159*Sin(b));
      glVertex3f(xLoc, 0.5*Cos(b)+0.5*Cos(a)-0.5, zLoc);
      // without this normal, the mound appears to have flat shading even when the mode is smooth shading
      glNormal3f(0.5*3.14159*Sin(a), 1.0, 0.5*3.14159*Sin(b+interval));
      glVertex3f(xLoc, 0.5*Cos(b+interval)+0.5*Cos(a)-0.5, zLoc+cartInterval); // the next row of z
      xLoc += cartInterval;
    }
    glEnd();
    zLoc += cartInterval;
  }
}

