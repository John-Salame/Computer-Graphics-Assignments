/*
 * Author: John Salame
 * Draw a scene with candy canes and an optional light
 */
#include "scenes.h"

/*
 * Draw a scene with candy canes and an optional light
 * @param dim: the dimensions of the viewport (determines the size of the base plate)
 * @param light: a boolean (1 to use lighting, 0 to not)
 * @param l0Position: the position of the light0
 * @param l1Position: the position of light1
 * @param day: a boolean; 1=daytime, 0=night-time
 */
void scene0(int dim, int light, float l0Position[4], float l1Position[4], int day) { 
  // store our view of the projection by pushing the matrix
  glPushMatrix();

  // If we want lighting, place a ball at the light and enable lighting.
  glDisable(GL_LIGHTING);
  if(light) {
    glPushMatrix();
    // create the plane for the sun and moon to sit on; assume 40 degrees north latitude
    glRotatef(130, 1, 0, 0);
    ball(l0Position[0], l0Position[1], l0Position[2], 1.5); // Sun
    ball(l1Position[0], l1Position[1], l1Position[2], 1.0); // Moon
    if(day) {
      glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
    }
    else {
      glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
    }
    // enable lighting
    glEnable(GL_LIGHTING);
    glPopMatrix();
  }

  // create the base plate
  float dayGreen[] = {50/255.0, 205/255.0, 50/255.0, 1.0}; // lime green color code
  float nightGreen[] = {34/255.0, 139/255.0, 34/255.0, 1.0}; // forest green color code
  float *baseGreen = 0;
  if(day)
    baseGreen = dayGreen;
  else
    baseGreen = nightGreen;
  glColor4fv(baseGreen);
  glMaterialfv(GL_FRONT, GL_AMBIENT, baseGreen);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, baseGreen);
  glNormal3f(0, 1, 0); // the normal vector of the ground is up
  glBegin(GL_QUADS);
  glVertex3f(0.8*dim, 0, 0.8*dim); //make sure to have CCW winding
  glVertex3f(0.8*dim, 0, -0.8*dim);
  glVertex3f(-0.8*dim, 0, -0.8*dim);
  glVertex3f(-0.8*dim, 0, 0.8*dim);
  glEnd();
  // place some candy canes
  CandyCane(0.8, 4.0, 1.5, 180);
  glTranslatef(2.0, -1.0, 2.0); // offset from the previous candy cane
  glRotatef(60, 0, 1.0, 0); //rotate around the candy cane axis
  CandyCane(1.0, 7.0, 1.3, 160);
  // put me back at the origin of the projection by popping and pushing
  glPopMatrix();
  glPushMatrix();
  // create a snow mound at (5, 0, -8)
  glTranslatef(5, 0, -8);
  glPushMatrix(); // so I can make a small candy cane at this position soon
  glScalef(6.0, 2.0, 5.0);
  threeDCos();
  glPopMatrix();
  glPushMatrix();
  // make a small tilted candy cane in the snow pile
  glTranslatef(0.1, 0.5, 0);
  glRotatef(-30, 0, 0, 1);
  glRotatef(-30, 1, 0, 0);
  CandyCane(0.2, 1.2, 0.3, 160);
  glPopMatrix(); // return to the state of the first snow pile
  // make one more snow pile, but smaller and taller
  glTranslatef(-3, 0, -2);
  glScalef(4.5, 3.5, 5.0);
  threeDCos();
  // return to the origin of the scene and save it again
  glPopMatrix();
  glPushMatrix();
  // make a large, shallow snow pile between the large candy canes and the overhead view
  glTranslatef(-6, 0, 8);
  glPushMatrix();
  glScalef(10, 2.5, 10);
  threeDCos();
  glPopMatrix();
  // make a candy cane in the large snow pile
  glTranslatef(1, 0, 0);
  glRotatef(-15, 0, 0, 1);
  glRotatef(-120, 0, 1, 0);
  CandyCane(0.4, 2, 0.6, 210);
  glPopMatrix(); // return to the caller's transform
}

