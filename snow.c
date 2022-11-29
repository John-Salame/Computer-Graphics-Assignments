/*
 * Author: John Salame
 * A file which contains object definitions for snow piles and (in the future) snowflakes
 */
#include "objects.h"

/*
 * The positive part of a 3D Cosine wave centered at (0, 0)
 * Equation: y = 0.5*Cos(180*x)+0.5*Cos(180*z)-0.5, domain x e [-0.5, 0.5], z e [-0.5, 0.5]
 * Good for making mounds of snow
 * @param unsigned int texName: The name of the texture to use
 */
void threeDCos(unsigned int texName, unsigned int normalMapName, unsigned int programName) {
  // prepare textures
  glUseProgram(programName);
  unsigned int texLoc = glGetUniformLocation(programName, "tex");
  unsigned int normalLoc = glGetUniformLocation(programName, "normalMap");
  // bind the uniform samplers to texture units (0 for decal, 1 for normal map)
  ErrCheck("snow pile getting uniforms");
  glUniform1i(texLoc, 0);
  glUniform1i(normalLoc, 1);
  ErrCheck("snow pile setting uniforms");
  // texture settings
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, normalMapName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // only reflect in the vertical direction (z on the snow pile)
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, texName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // only reflect in the vertical direction (z on the snow pile)
  ErrCheck("snow pile setting textures");
  const int reps = 2; // texture repetitions
  const double min = -0.5;
  // we do not need a variable for it, but the max is 0.5
  const int interval = 15; // angular interval
  const double cartInterval = (double) interval / 180; // cartesian interval (space between points)
  double xLoc = min;
  double zLoc = min;
  // apply lighting
  float white[] = {1.0, 1.0, 1.0, 1.0}; // Note: The lighting will be such that the snow appears gray at night (lighting enabled = night)
  float blue[] =  {0.1, 0.2, 0.4, 1.0};
  glColor4fv(white); // without the color, the snow mound is white. It seems like the materials don't do anything at all to the color.
  glMaterialfv(GL_FRONT, GL_AMBIENT, white); // snow looks blue at night
  glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, blue);
  ErrCheck("snow pile before attributes");
  // enable the vertex attribute for Tangent vector
  unsigned int tangentIndex = glGetAttribLocation(programName, "InTangent");
  unsigned int bitangentDirIndex = glGetAttribLocation(programName, "BitangentDir");
  glEnableVertexAttribArray(tangentIndex);
  glEnableVertexAttribArray(bitangentDirIndex);
  glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
  ErrCheck("snow pile setup");
  // b is the z position
  for(int b = -90; b < 90; b+= interval) {
    xLoc = min;
    glBegin(GL_QUAD_STRIP);
    // a is the x position
    for(int a = -90; a <= 90; a += interval) {
      // point 1
      // the normal is the gradient of the function, where the function value is y
      glVertexAttrib3f(tangentIndex, 1.0, -0.5*3.14159*Sin(a), 0.0);
      glNormal3f(0.5*3.14159*Sin(a), 1.0, 0.5*3.14159*Sin(b));
      glTexCoord2f(reps*(xLoc+0.5), reps*(zLoc+0.5));
      glVertex3f(xLoc, 0.5*Cos(b)+0.5*Cos(a)-0.5, zLoc);
      // point 2
      // without this normal, the mound appears to have flat shading even when the mode is smooth shading
      //glVertexAttrib3f(tangentIndex, 1.0, -0.5*3.14159*Sin(a), 0.0);
      glNormal3f(0.5*3.14159*Sin(a), 1.0, 0.5*3.14159*Sin(b+interval));
      glTexCoord2f(reps*(xLoc+0.5), reps*(zLoc+cartInterval+0.5));
      glVertex3f(xLoc, 0.5*Cos(b+interval)+0.5*Cos(a)-0.5, zLoc+cartInterval); // the next row of z
      xLoc += cartInterval;
    }
    glEnd();
    zLoc += cartInterval;
  }
  glUseProgram(0); // reset to fixed pipeline, so it will be the default program
  ErrCheck("snow pile");
}

