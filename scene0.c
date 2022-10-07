/*
 * Author: John Salame
 * Draw a scene with candy canes and an optional light
 */

#include "myCSCI5229.h"
#include "scenes.h"

/*
 * Draw a scene with candy canes and an optional light
 * @param light: a boolean (1 to use lighting, 0 to not)
 * @param Position: The position of the light
 */
void scene0(int light, float Position[4], int dim) { 
  // store our view of the projection by pushing the matrix
  glPushMatrix();

  // Draw a circle where the light will be. Draw it before lighting is enabled.
  if(light) {
    glDisable(GL_LIGHTING);
    Circle(15, 1, Position[0], Position[1], Position[2]);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    glEnable(GL_LIGHTING);
  }

  // create the base plate
  float forestGreen[] = {34/255.0, 139/255.0, 34/255.0, 1.0};
  glColor4fv(forestGreen);
  glMaterialfv(GL_FRONT, GL_AMBIENT, forestGreen);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, forestGreen);
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

