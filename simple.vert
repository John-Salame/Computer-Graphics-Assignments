//   A vertex shader that only passes position and color to the fragment shader.
//   This is used in the blizzard shader
//   Author: John Salame
#version 120

void main()
{
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
