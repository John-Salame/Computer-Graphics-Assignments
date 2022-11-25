/*
 * Original Author: Vlakkies
 * Modified by: John Salame
 * This is a library of helper functions, with the Projection function modified by John.
 */
#ifndef myCSCI5229
#define myCSCI5229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  cos and sin in degrees
#define MY_PI 3.14159265
#define Cos(th) cos(MY_PI/180*(th))
#define Sin(th) sin(MY_PI/180*(th))

// put this around every file you compile with C++
#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
//void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
//int  LoadOBJ(const char* file);

// stuff related to loading in shader programs
char* ReadText(char *file);
void PrintShaderLog(int obj,char* file);
void PrintProgramLog(int obj);
int CreateShader(GLenum type,char* file);
int CreateShaderProg(char* VertFile,char* FragFile);

#ifdef __cplusplus
}
#endif

#endif
