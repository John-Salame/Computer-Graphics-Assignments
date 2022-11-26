//  Per Pixel Lighting shader with texture
//  Modified from Example 25
#version 120

varying vec4 P;
varying mat3 TangentSpace;

void main()
{
   //  Vertex location in modelview coordinates
   P = gl_ModelViewMatrix * gl_Vertex;
   //  Tangent Space
   //  I will normalize Tangent Space vectors in the fragment shader after they are interpolated.
   vec3 Normal = gl_NormalMatrix * gl_Normal;
   vec3 Tangent = vec3(gl_ModelViewMatrix * vec4(1.0, 0.0, 0.0, 0.0));
   vec3 Bitangent = cross(Normal, Tangent);
   TangentSpace[0] = Tangent;
   TangentSpace[1] = Bitangent;
   TangentSpace[2] = Normal;
   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_FrontColor = gl_Color;
}
