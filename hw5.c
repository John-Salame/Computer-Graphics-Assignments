/*
 * John Salame
 * CSCI 5229 Computer Graphics
 * Homework 5 - Lighting
 * Due 10/6/22
 */

#include "myCSCI5229.h"
#include "scenes.h"

// Forward declarations
void updateFpVecs();

// Begin global variables
// Normally, it's good to initialize everything. However, I will initialize things in init() so I can make a reset functionality.
int th; // angle around y-axis
int ph; // angle around x-axis
double dim; // width and height of the orthographic projection
int mode; // begin in orthogonal projection
int fpTh; // first-person theta
int fpPh; // first-person phi
double forward[3]; // first-person forward unit vector
double up[3]; // first-person up unit vector
double eye[3]; // first-person eye position
double walk; // how much to walk with each step
// Projection-related stuff
int fov;
double asp; // aspect ratio, used to keep the proportions of an object constant when resizing the windowa

// lighting related globals
int ambient = 10;
int diffuse = 20; // low diffuse for night-time lighting
int light = 1; // bool to enable lighting
int scene = 0; // choose which scene to render

// BEGIN UTILITY FUNCTIONS

// set the global variables to reflect the default view of scene 0
// Note: Implementation is tightly coupled to the global variables of the main script
void initScene0() {
  th = 55;
  ph = 20;
  dim = 20.0;
  mode = 0;
  // Start with an angle away from the standard forward.
  // With this style, I can have a nice starting view also with easy calculations 
  // because I only have one dimension on forward to worry about.
  fpTh = -290;
  fpPh = 5;
  forward[0]=0; forward[1]=0; forward[2]=-1;
  up[0]=0; up[1]=1; up[2]=0;
  eye[0]=12.046; eye[1]=2.0; eye[2]=4.404;
  walk = 0.25;
  fov = 55;
  updateFpVecs(); // recalculate forward and up using fpTh and fpPh
}

// depends on "scene" global variable
void init() {
  switch(scene) {
    case 0:
      initScene0();
      break;
    default:
      Fatal("Scene %d does not exist", scene);
  }
}

/* Update the forward and up vectors for first-person mode */
void updateFpVecs() {
  // The following calculations are based on rotating (0, 0, -1) around x-axis and then y-axis.
  // However, since we want left to turn left (counter-clockwise) and right to turn right (clockwise),
  // we invert the change in first-person theta so the right key makes a negative change to fpTh,
  // which makes a positive change in x from the initial forward direction.
  forward[0] = -Sin(fpTh)*Cos(fpPh);
  forward[1] = Sin(fpPh);
  forward[2] = -Cos(fpTh)*Cos(fpPh);
  // Now, calculate the up vector under the same assumptions
  up[0] = Sin(fpTh)*Sin(fpPh);
  up[1] = Cos(fpPh);
  up[2] = Cos(fpTh)*Sin(fpPh);
}

/* Change the projection settings */
void Project() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  switch(mode) {
    case 0:
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
      ErrCheck("Project Orthogonal");
      break;
    case 1:
      gluPerspective(fov,asp,dim/4,4*dim);
      ErrCheck("Project Perspective");
      break;
    case 2:
      gluPerspective(fov,asp,dim/32,2*dim); // I change the near and far for first-person so we can get a good look.
      ErrCheck("Project First-Person");
      break;
    default:
      Fatal("This mode should not exist: mode %d", mode);
    // Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    // Undo previous transformations
    glLoadIdentity();
    ErrCheck("Project");
  }
}

/* Helper function to display the coordinate axes */
void displayAxes() {
  /* To-Do: Improve by pushing and popping the current transform matrix */
  // x-axis
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0.5*dim, 0, 0); 
  glEnd();
  glRasterPos3f(0.5*dim, 0, 0);
  Print("X");
  //y-axis
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0.5*dim, 0);
  glEnd();
  glRasterPos3f(0, 0.5*dim, 0);
  Print("Y");
  // z-axis
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 0.5*dim);
  glEnd();
  glRasterPos3f(0, 0, 0.5*dim);
  Print("Z");
  ErrCheck("display axes");
}



// BEGIN OBJECT DEFINITIONS

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



// BEGIN CALLBACK FUNCTIONS


void display() {
  double lookAtC[3];
  double Ex;
  double Ey;
  double Ez;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // prevent a bug from double rendering the scene after a reshape
  glPushMatrix();
  switch(mode) {
    // Orthogonal Overhead
    case 0:
      glRotatef(ph, 1, 0, 0);
      glRotatef(th, 0, 1, 0);
      ErrCheck("Display Orthogonal");
      break;
    // Perspective Overhead
    case 1:
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
      ErrCheck("Display Perspective");
      break;
    // Perspective First Person
    case 2:
      for(int i=0; i<3; i++)
        lookAtC[i] = eye[i]+forward[i];
      gluLookAt(eye[0],eye[1],eye[2], lookAtC[0],lookAtC[1],lookAtC[2], up[0],up[1],up[2]);
      ErrCheck("Display Fisrt-Person");
      break;
    default:
      Fatal("This mode should not exist: mode %d", mode);
  }
  glDisable(GL_LIGHTING);
  displayAxes();

  // now, start lighting settings and then draw objects
  float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
  //float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.00*diffuse ,1.0}; // yellow light, like a lamp
  float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.05*diffuse ,1.0}; // yellow light with a tinge of blue (moonlight?); allows the snow's blue properties to show
  float Position[4]; // set the position of light 0 lower down, based on the scene we want to render
  
  // enable lighting and unit vector normals
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  // location of viewer for specular light calculations. I'm not sure what the difference is, even from testing example 13.
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL); // without this enabled, the glColor4fv does not apply, but the materials do
  // use light 0
  glEnable(GL_LIGHT0);
  // set the intenisty and color of each type of lighting for light 0, plus set the position
  glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
  // choose flat or smooth lighting
  glShadeModel(GL_SMOOTH);
  
  // AT THIS POINT, LIGHT0 HAS NO POSITION! HANDLE THAT IN THE SCENE YOU DRAW.
  if(scene == 0) {
    Position[0] = dim*Cos(th);
    Position[1] = dim*Sin(ph);
    Position[2] = dim*-Sin(th)*Cos(ph);
    Position[3] = 1.0;
  }
  
  // Create the objects in the scene
  scene0(light, Position, dim);

  // Cleanup: reset the matrix in the state machine (this should undo any viewpoint or projection)
  glPopMatrix();
  // render the scene
  ErrCheck("display");
  glFlush();
  glutSwapBuffers();
}

/* React to key presses */
void special(int key, int x, int y) {
  int dir = 0; // positive for positive theta increase, negative for a theta decrease
  int dir2 = 0; //same as dir, but for up and down arrow keys
  int thRate = 5; // degrees of rotation per arrow key press
  int phRate = 5;
  if(key == GLUT_KEY_RIGHT)
    dir = 1;
  else if(key == GLUT_KEY_LEFT)
    dir = -1;
  else if(key == GLUT_KEY_UP)
    dir2 = 1;
  else if(key == GLUT_KEY_DOWN)
    dir2 = -1;
  
  switch(mode) {
    // Orthogonal Overhead
    case 0:
      // no break; intentional fall through so th and ph correlate for both overhead modes
    // Perspective Overhead
    case 1:
      th += dir * thRate;
      ph += dir2 * phRate;
      break;
    // Perspective First-Person
    case 2:
      // first person theta and phi, for turning your head
      fpTh -= dir * thRate;
      fpPh += dir2 * phRate;
      fpTh %= 360;
      fpPh %= 360;
      updateFpVecs(); // update forward and up
      break;
    default:
      Fatal("This mode should not exist: mode %d", mode);
  }
  //  Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
  // re-project
  Project();
  // re-render the screen
  glutPostRedisplay();
}

/* Keyboard keys, mainly for switching view mode and for walking */
void key(unsigned char ch,int x,int y) {
  int dir1 = 0; // left-right (right is positive)
  int dir2 = 0; // forward-back (forward is positive)
  double invMag = 0; // the inverse magnitude of a vector; multiply by this in order to normalize
  double fwX = 0; // x portion of forward vector projected onto ground and normalized
  double fwZ = 0; // z portion of forward vector projected onto ground and normalized
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  // reset the scene
  else if (ch == '0')
    init();
  // M: cycle to the next view mode (projection type)
  else if (ch == 'm' || ch == 'M')
    mode = (mode + 1) % 3;
  // Only allow planar movement in mode 2 (first-person)
  switch(mode) {
    // I wonder if this is a security vulnerability. Would people be able to arbitrarily put any mode number and jump to some random instruction and crash the program?
    case 2:
      if (ch == 'w' || ch == 'W')
        dir2 = 1;
      else if (ch == 'a' || ch == 'A')
        dir1 = -1;
      else if (ch == 's' || ch == 'S')
        dir2 = -1;
      else if (ch == 'd' || ch == 'D')
        dir1 = 1;
      // Now, move your character
      //   First, update your forward and up vector in case your first-person viewing angles changed
      updateFpVecs();
      //   Now, calculate the magnitude of forward parallel to the ground plane (x and z components)
      //   Please note that the right vector has this same magnitude and is perpendicular
      invMag = sqrt(1/(forward[0]*forward[0] + forward[2]*forward[2]));
      // Now normalize the components of the forward vector parallel to the ground and then scale by our walk step
      fwX = forward[0]*invMag*walk;
      fwZ = forward[2]*invMag*walk;
      // Now, change the x and z of the eye based on forward, right, and the direction we're going.
      //   Fwd = [fwX, 0, fwZ], Rt = [-fwZ, 0 fwX]
      eye[0] += dir2*fwX - dir1*fwZ;
      eye[2] += dir2*fwZ + dir1*fwX;
      // enable the below line for debugging. I will comment it out or add it back instead of adding a debug mode to the Makefile.
      //printf("eye [%f, %f, %f] fwd [%f, %f, %f], th %d ph %d\n", eye[0], eye[1], eye[2], forward[0], forward[1], forward[2], fpTh, fpPh);
      break;
  }
  // re-project
  Project();
  // re-render the screen
  glutPostRedisplay();
}

void reshape(int width, int height) {
  // Set viewport as entire window
  glViewport(0, 0, RES*width, RES*height);
  //  unit cube adjusted for aspect ratio
  asp = (height>0) ? (double)width/height : 1;
  // set the projection to orthogonal or perspective, using the aspect ratio
  Project();
}


int main(int argc, char** argv) {
  init();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("John Salame HW5: Lighting");
#ifdef USEGLEW
  //  Initialize GLEW
  if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
  // Register the callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutReshapeFunc(reshape);
  // Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
  // Finally, allow the window to draw
  ErrCheck("init");
  glutMainLoop();
  return 0;
}

