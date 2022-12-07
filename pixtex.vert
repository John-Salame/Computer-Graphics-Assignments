//  Per Pixel Lighting shader with texture
//  Author: John Salame
//  Modified from Example 25
#version 120

varying vec4 P;
varying vec3 Normal;

void main()
{
   //  Vertex location in modelview coordinates
   P = gl_ModelViewMatrix * gl_Vertex;
   //  Normal
   Normal = gl_NormalMatrix * gl_Normal;
   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_FrontColor = gl_Color;
}
