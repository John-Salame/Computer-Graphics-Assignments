/*
 * John Salame
 * CSCI 5229 Computer Graphics
 * Final Project
 * Due 12/6/22
 */

// Includes
#include "myCSCI5229.h" // OpenGL and some helper functions come from here
#include "scenes.h" // also includes individual objects we can draw

// Macros
#define NUM_TEX 6 // number of textures
#define NUM_PROGS 4 // number of shader programs
#define NUM_NORMAL_MAPS 4

// Forward declarations
void updateFpVecs();
void idle();

// Begin global variables
// Normally, it's good to initialize everything. However, I will initialize things in init() so I can make a reset functionality.
int th; // angle around y-axis
int ph; // angle around x-axis
// projection variables
double dim; // width and height of the orthographic projection
int mode; // begin in first-person projection
int fov;
double asp; // aspect ratio, used to keep the proportions of an object constant when resizing the windowa
// first-person variables
int fpTh; // first-person theta
int fpPh; // first-person phi
double forward[3]; // first-person forward unit vector
double up[3]; // first-person up unit vector
double eye[3]; // first-person eye position
double walk; // how much to walk with each step
// lighting related globals
int day; // flag for day/night, sun/moon
int ambient;
int diffuse; // low ambient and diffuse for night-time lighting
int specular;
int light; // bool to enable lighting
int lTh; // theta and phi of the light; sun and moon use only theta.
float lZ; // z value of the light
// other display variables
int scene = 0; // choose which scene to render
int numScenes = 4;
int controlLight = 0; // when enabled, you can stop the light and move it around with arrow keys
int pause = 0; // when enabled, stop the light
int axes;
// texture settings
int useTexture; // flag whether texture is enabled (1) or diabled (0); set in init()
unsigned int texture[NUM_TEX];  //  Texture names
unsigned int normalMaps[NUM_NORMAL_MAPS];
// shaders
unsigned int shaders[NUM_PROGS]; // this holds the shader programs


// BEGIN UTILITY FUNCTIONS

void changeScene(int dir) {
  scene = (scene + numScenes + dir) % numScenes; // dir can be 1 or -1 to go to next or previous scene
}

// set the global variables to reflect the default view of scene 0
// Note: Implementation is tightly coupled to the global variables of the main script
void initScene0() {
  // overrides for scene 0
  mode = 2; // first-person perspective
  dim = 20.0;
  th = 55;
  ph = 20;
  // Start with an angle away from the standard forward.
  // With this style, I can have a nice starting view also with easy calculations 
  // because I only have one dimension on forward to worry about.
  fpTh = -30;
  fpPh = 5;
  eye[0]=-10.0; eye[1]=2.0; eye[2]=14.0;
  updateFpVecs(); // recalculate forward and up using fpTh and fpPh
  day = 1;
}

// depends on "scene" global variable
void init() {
  // defaults for every scene
  mode = 0; // orthogonal
  axes = 0; // do not display axes
  dim = 5.0;
  th = 0;
  ph = 0;
  fov = 55;
  walk = 0.25;
  fpTh = 0;
  fpPh = 0;
  forward[0]=0; forward[1]=0; forward[2]=-1;
  up[0]=0; up[1]=1; up[2]=0;
  eye[0]=0; eye[1]=0; eye[2]=dim;
  // light settings (other than day-night, which are in display()) 
  light = 1;
  lTh = 0;
  lZ = 0;
  useTexture = 1;
  switch(scene) {
    case 0:
      initScene0();
      break;
    case 1:
      // do nothing
      break;
    case 2:
      // do nothing
      break;
    case 3:
      // do nothing
      break;
    default:
      Fatal("Scene %d does not exist\n", scene);
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
  }
  // Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  // Undo previous transformations
  glLoadIdentity();
  ErrCheck("Project");
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


// BEGIN CALLBACK FUNCTIONS


void display() {
  double lookAtC[3];
  double Ex;
  double Ey;
  double Ez;
  // decide if it's day or night (for scene 0), then choose the background color
  lTh %= 360; // keep it in +/- 360 degrees 
  if ((lTh >= 0 && lTh < 180) || lTh <= -180) {
    day = 1; //day
  }
  else {
    day = 0; // night
  }
  // choose the background color
  if (scene == 0 && day) {
    glClearColor(0.2, 0.4, 1.0, 1.0); // blue sky
  }
  else {
    glClearColor(0, 0, 0, 1); // black background by default
  }
  // clear the buffers and apply the background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_CULL_FACE);
  // set the view angle or first-person perspective view
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

  // in case we want to draw anything like axes which do not need lighting or textures
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glUseProgram(0); // for drawing axes
  if (axes)
    displayAxes();

  glUseProgram(shaders[0]); // use the shader program that mimics the pipeline

  // start texture settings
  if (useTexture) {
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE); //GL_MODULATE
  }
  // now, start lighting settings and then draw objects
  // first, get rid of any left-over material properties from the state
  float zero[] = {0, 0, 0, 1.0}; // resets the material colors
  glMaterialfv(GL_FRONT, GL_AMBIENT, zero);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, zero);
  glMaterialfv(GL_FRONT, GL_SPECULAR, zero);

  // set the lights to zero so the shader will not add color from lights that are disabled
  glLightfv(GL_LIGHT0, GL_AMBIENT, zero);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, zero);
  glLightfv(GL_LIGHT0, GL_SPECULAR, zero);
  glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, zero);
  glLightfv(GL_LIGHT1, GL_SPECULAR, zero);

  // determine if it is night or day (currently only matters for scene 0)
  // set night light properties
  if(scene == 0 && day == 0) {
    ambient = 10;
    diffuse = 20;
    specular = 20;
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1); // only use the moon at night-time
  }
  else {
    // find daytime light settings in init()
    ambient = 30;
    diffuse = 50;
    specular = 20;
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1); // only use light 1 on scene 0 night-time
  }
  // set the light properties
  float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.02*ambient ,1.0}; // slightly blue since the sky is blue
  float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
  float Specular[] = {0.01*specular, 0.01*specular, 0.01*specular, 1.0};
  // set the light positions
  // l0Position is light0 rotation from (1, 0)
  float l0Position[] = {dim*Cos(lTh), 0, -dim*Sin(lTh), 1.0}; // rotate light around the y-axis starting from (1, 0, 0)
  float l1Position[] = {-dim*Cos(lTh), 0, dim*Sin(lTh), 1.0}; // opposite side from l0; represents the moon in scene 1
  
  // create unit vector normals
  glEnable(GL_NORMALIZE);
  // location of viewer for specular light calculations. I'm not sure what the difference is, even from testing example 13.
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL); // without this enabled, the glColor4fv does not apply, but the materials do
  
  // set the intenisty and color of each type of lighting for lights 0 and 1
  if(scene == 0 && day == 0) {
    // determine if it is night or day (currently only matters for scene 0)
    // set night light properties
    glLightfv(GL_LIGHT1, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Specular);
  }
  else {
    // day or scene other than scene 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
  }
  
  // choose flat or smooth lighting
  glShadeModel(GL_SMOOTH); //GL_SMOOTH or GL_FLAT
  
  // AT THIS POINT, LIGHT0 HAS NO POSITION! HANDLE THAT IN THE SCENE YOU DRAW.
  if(scene == 0) {
    // Create the objects in the scene
    scene0(dim, light, l0Position, l1Position, day, texture, normalMaps, shaders);
  }
  // display one of the simple scenes with a light rotating around an object
  else {
    glPushMatrix(); // in case I choose to do any rotation or scaling in the scenes
    // raise or lower the light
    l0Position[1] += lZ;
    // allow the light source balls to be drawn properly
    GLboolean useCulling = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glUseProgram(0); // allow lights source ball to be drawn without lighting
    if(light) {
      ball(l0Position[0], l0Position[1], l0Position[2], 0.5);
      glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
      // finally enable lighting
      glEnable(GL_LIGHTING);
    }
    // reset face culling if necessary
    if(useCulling) {
      glEnable(GL_CULL_FACE);
    }
    // now choose object based on scene
    if (scene == 1) {
      CandyCane(0.5, 1.0, 1.0, 180, texture[3], normalMaps[3], shaders[2]);
    }
    else if (scene == 2) {
      threeDCos(texture[2], normalMaps[2], shaders[2]); // snow pile using snow3.bmp texture
    }
    else if (scene == 3) {
      bench(0.8*dim, 0.6*dim, 0.4*dim, texture[3], normalMaps[3], shaders[3]); // use "normal map mix" shader
    }
    glPopMatrix();
  } 


  // Display parameters
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glColor3f(1.0, 1.0, 1.0);
  glUseProgram(0);
  glWindowPos2i(5,5);
  Print("Light: %d; Control light: %d; Pause: %d", light, controlLight, pause);
  glWindowPos2i(5, 25);
  Print("light theta: %d; light z: %f", lTh, lZ);

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
  
  // any arrow key can change the scene; bring up scene with default parameters.
  if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
    changeScene(dir + dir2);
    init();
  }

  // in "control light" mode, arrow keys move the light
  if (controlLight) {
    lTh += dir * thRate;
    lZ += dir2 * walk; // raise or lower the light
  }
  // allow the camera to move
  else {
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
  // X: toggle axes
  else if (ch == 'x' || ch == 'X')
    axes = !axes;
  // L: toggle the light
  else if (ch == 'l' || ch == 'L')
    light = !light;
  else if (ch == 't' || ch == 'T')
    useTexture = !useTexture;
  // C or O: control the light (stop the light from moving and allow you to move the light using arrow keys)
  else if (ch == 'c' || ch == 'C' || ch == 'o' || ch == 'O') {
    controlLight = !controlLight;
    pause = 0; // "Control Light" and "pause" turn each other off
    if (controlLight) {
      glutIdleFunc(NULL);
    }
    else
      glutIdleFunc(idle);
  }
  // P: pause the movement of the light but still allow the user to rotate the screen
  else if (ch == 'p' || ch == 'P') {
    pause = !pause;
    controlLight = 0; // "Control Light" and "pause" turn each other off
    if (pause)
      glutIdleFunc(NULL);
    else {
      glutIdleFunc(idle);
    }
  } // end of pause logic

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

void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   // slow down the light's rotation on scene 0 (day and night cycle)
   if (scene == 0)
     lTh = fmod(30*t,360.0);
   else
     lTh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
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
  glutInitWindowSize(600, 400);
  glutCreateWindow("John Salame Final Project");
#ifdef USEGLEW
  //  Initialize GLEW
  if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
  // Register the callback functions
  glutDisplayFunc(display);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  // Enable Z-buffer depth test
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  // Load textures
  texture[0] = LoadTexBMP("noTex.bmp");
  texture[1] = LoadTexBMP("snow.bmp");
  texture[2] = LoadTexBMP("snow3.bmp");
  texture[3] = LoadTexBMP("candyCane.bmp");
  texture[4] = LoadTexBMP("grass.bmp"); //Attribution: <a href="https://www.freepik.com/free-photo/green-grass-field-background_991898.htm#&position=0&from_view=author">Image by awesomecontent</a> on Freepik
  texture[5] = LoadTexBMP("pi.bmp");
  // Load normal maps
  normalMaps[0] = LoadTexBMP("noNormal.bmp");
  normalMaps[1] = LoadTexBMP("snow_normal.bmp");
  normalMaps[2] = LoadTexBMP("snow3_normal.bmp");
  normalMaps[3] = LoadTexBMP("candyCane_normal.bmp");
  // Create shader programs
  shaders[0] = CreateShaderProg("texture.vert", "texture.frag"); // mimic standard pipeline
  shaders[1] = CreateShaderProg("pixtex.vert", "pixtex.frag"); // per-pixel lighting
  shaders[2] = CreateShaderProg("normalMap.vert", "normalMap.frag");
  shaders[3] = CreateShaderProg("normalMap.vert", "normalMapMix.frag");
  // Finally, allow the window to draw
  ErrCheck("init");
  glutMainLoop();
  return 0;
}

