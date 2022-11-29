//  Per Pixel Lighting shader with texture and normal map
//  Modified from Per-Pixel Lighting
#version 120

// supplying attribute: https://stackoverflow.com/questions/27671741/how-are-attributes-passed-to-vertex-shader-in-glsl
attribute vec3 InTangent;
attribute float BitangentDir; // make this -1 if Bitangent = Tangent cross Normal
varying vec4 P;
varying mat3 TangentSpace;

void main()
{
   //  Vertex location in modelview coordinates
   P = gl_ModelViewMatrix * gl_Vertex;
   //  Tangent Space
   //  I will normalize Tangent Space vectors in the fragment shader after they are interpolated.
   vec3 Tangent = vec3(gl_ModelViewMatrix * vec4(InTangent, 0.0));
   vec3 Normal = gl_NormalMatrix * gl_Normal;
   vec3 Bitangent = BitangentDir * cross(Normal, Tangent);
   TangentSpace[0] = Tangent;
   TangentSpace[1] = Bitangent;
   TangentSpace[2] = Normal;
   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_FrontColor = gl_Color;
}
