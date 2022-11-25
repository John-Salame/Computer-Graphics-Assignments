//  Phong lighting
#version 120

const int numLights = 2;

void main()
{
   //  P is the vertex coordinate on body
   vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
   //  N is the object normal at P
   vec3 N = normalize(gl_NormalMatrix * gl_Normal);
   //  V is the view vector (eye at the origin)
   vec3 V = normalize(-P);

   // Calculate color from each light and add it to myColor
   vec4 myColor = vec4(0.0, 0.0, 0.0, 1.0);
   for(int i = 0; i < numLights; i++) {
      //  Light Position for light 0
      vec3 LightPos = vec3(gl_LightSource[i].position);
      //  L is the light vector
      vec3 L = normalize(LightPos - P);
      //  R is the reflected light vector R = 2(L.N)N - L
      vec3 R = reflect(-L, N);
      
      //  Diffuse light intensity is cosine of light and normal vectors
      float Id = max(dot(L,N) , 0.0);
      //  Shininess intensity is cosine of light and reflection vectors to a power
      float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , gl_FrontMaterial.shininess) : 0.0;

      //  Vertex color from current light in the loop
      myColor += (gl_FrontLightProduct[i].ambient
               + Id*gl_FrontLightProduct[i].diffuse
               + Is*gl_FrontLightProduct[i].specular);
   } // end of loop over lights

   gl_FrontColor = myColor;
   // My own addition to try and mimic Modulate texture mode (otherwise, untextured objects appear white)
   gl_FrontColor *= gl_Color;

   //  Texture
   // Setting texture and normal map: https://stackoverflow.com/questions/25252512/how-can-i-pass-multiple-textures-to-a-single-shader
   gl_TexCoord[0] = gl_MultiTexCoord0;

   //  Return fixed transform coordinates for this vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
