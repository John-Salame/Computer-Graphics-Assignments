/* 
 * Author: John Salame
 * Description: A file containing the candy cane object and related shapes
 */
#include "objects.h"

/* Draw a circle with intervals of circlePrecision degrees and radius r at origin (ox, oy, oz) */
void Circle(float circlePrecision, float r, float ox, float oy, float oz) {
  float white[] = {1, 1, 1, 1};
  float zero[] = {0, 0, 0, 1.0}; // resets the material colors
  glColor4fv(white);
  glMaterialfv(GL_FRONT, GL_AMBIENT, white);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
  glNormal3f(0, 0, 1); // normal is just the front plane's normal, which is z for this circle
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.5, 0.5);
  glVertex3f(ox, oy, oz); // center of triangle fan
  for(int i=0; i<=360; i+=circlePrecision) {
    glTexCoord2f(0.5*(1+Sin(i)), 0.5*(1-Cos(i)));
    glVertex3f(ox+r*Cos(i), oy+r*Sin(i), oz);
  }
  glEnd();
  ErrCheck("circle");
}

/* Helper function for candy cane, makes a cylinder wall with radius crossRad and height straightHeight */
void RedStripedCylinderWall(int circlePrecision, float crossRad, float straightHeight) {
  glPushMatrix();
  glScalef(crossRad, crossRad, straightHeight);
  float quadHeight = 0.2; // split the cylinder length-wise into multiple quads so lighting works close-up
  float nonRed = 1.0; // 1.0 when white stripe, 0.0 when red stripe
  float myColor[4];
  myColor[0] = 1.0;
  myColor[3] = 1.0;
  // draw the object
  for(float j=0; j<1.0; j+=quadHeight) {
    nonRed = 1.0; // prevent a bug that misaligned the stripes on the next layer of quads
    glBegin(GL_QUAD_STRIP);
    for(int i=0; i<=360; i+=circlePrecision) {
      myColor[1] = nonRed; myColor[2] = nonRed;
      glColor4fv(myColor); // I need an even number of rectangles for the color to start and end on red
      nonRed = !nonRed; // binary flip from 0 to 1 or 1 to 0
      glMaterialfv(GL_FRONT, GL_AMBIENT, myColor);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, myColor);
      glNormal3f(Cos(i), Sin(i), 0); // here, the normal vector is along the radius; normal is same for both vertices
      glVertex3f(Cos(i), Sin(i), j+quadHeight);
      glVertex3f(Cos(i), Sin(i), j);
    }
    glEnd();
  }
  glPopMatrix();
}

/* 
 * Create a pseudo-cylinder segment of the hook part of the candy cane
 * @param int circlePrecision - how many degrees a quad takes up, and also how many degrees of the curve a hook segment spans
 * @param float crossRad - the radius of the circular cross section
 * @param float hookRad - the radius of the hook curve (distance from the center of the curve to the center of the circular cross section)
 */
void RedStripedHookSegment(int circlePrecision, float crossRad, float hookRad) {
  glPushMatrix();
  float nonRed = 1.0;
  float myColor[4];
  myColor[0] = 1.0;
  myColor[3] = 1.0;
  // prepare the calculations and draw the object
  int hookDeg = circlePrecision;
  float secantAngle = (float) hookDeg/2; // x in the image hookProof.JPG
  glBegin(GL_QUAD_STRIP);
  // Draw the quads, which have variable height based on how far out they are
  for(int i=0; i <=360; i+=hookDeg) {
    myColor[1] = nonRed; myColor[2] = nonRed;
    glColor4fv(myColor); // I need an even number of rectangles for the color to start and end on red
    nonRed = !nonRed; // binary flip from 0 to 1 or 1 to 0
    // the normal vector is orthogonal to the tangent line, so it's simple (unlike the plane which follows a secant line).
    glMaterialfv(GL_FRONT, GL_AMBIENT, myColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, myColor);
    // start creating the shape
    float x = crossRad*Cos(i);
    float radius = hookRad-x;
    // The ordering of the points is counter-clockwise. That means we draw the more complicated point first.
    
    // Here we explain what is shown in my proof, hookProof.JPG
    // The "top" of the "cylinder" is found by following the secant line from the current hook segment to the start of the next hook segment.
    // The equation to do this is 2*hookRad*sin(hookDeg/2) and the direction of the secant line is the tangent line rotated by (hookDeg/2).
    // We rotate (0, 0, secantLength) counter-clockwise around the y-axis (positive) by (hookDeg/2).
    // From the 3D rotation matrix, sin(hookDeg/2) is (z*sin(hookDeg/2), 0, z*cos(hookDeg/2))
    //   = (secantLength*sin(hookDeg/2), 0, secantLength*cos(hookDeg/2)).
    //   We call hookDeg/2 the secantAngle.
    float secantLength = 2*radius*Sin(secantAngle);
    // rotate the normal vector by hookDeg along the y-axis
    //   x=x_old*cos(rot_angle), y=y_old, z=x_old*sin(rot_angle)
    // The x value x-old is x-hookRad, which is the same as -1*radius
    glNormal3f(Cos(i)*Cos(hookDeg), Sin(i), -Cos(i)*Sin(hookDeg)); 
    glVertex3f(-radius+secantLength*Sin(secantAngle), crossRad*Sin(i), secantLength*Cos(secantAngle));

    glNormal3f(Cos(i), Sin(i), 0); // here, the normal vector is along the radius
    glVertex3f(-radius, crossRad*Sin(i), 0);
  }
  glEnd();
  glPopMatrix();
}

/*
 * Create a candy cane with the bottom at the origin of the model matrix
 * @param float crossRad - radius of the cross section
 * @param float straightHeight - height of the tall straight part before the hook
 * @param float hookRad - the radius of the curve that makes the hook
 * @param float hookDeg - the length of the hook's sweep path in degrees (180 would make a half-circle)
 * @param unsigned int texName - name of the texture to use on the ends of the candy cane; I don't have texture on the main part because it looks terrible with the glossy look of the candy cane.
 */
void CandyCane(float crossRad, float straightHeight, float hookRad, int hookDeg, unsigned int texName) {
  glPushMatrix();
  glRotatef(-90, 1.0, 0, 0); // make it so y is up for the candy cane instead of z. I have this because I initially thought z was pointing up.
  // prepare texture
  glBindTexture(GL_TEXTURE_2D, texName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  int i = 0; // for the loop
  int circlePrecision = 15; // degrees per rectangle making up a cylinder
  // First, make a circle at the base of the candy cane
  // make sure the bottom of the circle is down
  glPushMatrix();
  glRotatef(180, 1.0, 0, 0);
  Circle(circlePrecision, crossRad, 0, 0, 0);
  glPopMatrix();
  // Now, make the tall straight cylinder
  RedStripedCylinderWall(circlePrecision, crossRad, straightHeight);
  // Now, make the hook
  glTranslatef(hookRad, 0, straightHeight);
  for(i=0; i<hookDeg; i+=circlePrecision) {
    RedStripedHookSegment(circlePrecision, crossRad, hookRad);
    glRotatef(circlePrecision, 0, 1.0, 0);
    //glRotatef(circlePrecision, 0, 1.0, 0); // rotate the curve origin so the next segment starts at the end of the previous segment
  }
  Circle(circlePrecision, crossRad, -hookRad, 0, 0);
  glPopMatrix();
  ErrCheck("candy cane");
}

