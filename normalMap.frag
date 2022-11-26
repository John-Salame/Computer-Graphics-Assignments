//  Per Pixel Lighting shader with texture and normal map
//  Modified from Per-Pixel Lighting
#version 120

const int numLights = 2;
varying vec4 P;
varying mat3 TangentSpace;

uniform sampler2D tex;
uniform sampler2D normalMap;

void main()
{
   //  N is the object normal; calculate it from the tangent space and the normal map
   // First, normalize the interpolated tangent space vectors
   mat3 TangentSpaceNormalized;
   for(int i = 0; i < 3; i++) {
      TangentSpaceNormalized[i] = normalize(TangentSpace[i]);
   }
   // now, get the normal map rgb data and then use it as weights for the tangent space vectors to compute a normal
   vec3 normalData = vec3(texture2D(normalMap,gl_TexCoord[0].xy));
   normalData = normalize(2*normalData - 1);
   vec3 N = normalize(TangentSpaceNormalized * normalData);
   //  V is the view vector (eye vector)
   vec3 V = normalize(-P.xyz);

   // Calculate color contribution for each light
   vec4 myColor = vec4(0.0, 0.0, 0.0, 1.0);
   for(int i = 0; i < numLights; i++) {
      //  L is the light vector
      vec3 L = normalize(gl_LightSource[i].position.xyz - P.xyz);
      //  R is the reflected light vector R = 2(L.N)N - L
      vec3 R = reflect(-L,N);

      //  Diffuse light is cosine of light and normal vectors
      float Id = max(dot(L,N) , 0.0);
      //  Specular is cosine of reflected and view vectors
      float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

      //  Sum color types
      myColor += (gl_FrontLightProduct[i].ambient
               + Id*gl_FrontLightProduct[i].diffuse
               + Is*gl_FrontLightProduct[i].specular);
   }
   myColor.w = 1.0;
   // My own addition to try and mimic Modulate texture mode (otherwise, untextured objects appear white)
   myColor *= gl_Color;

   //  Apply texture
   gl_FragColor = myColor * texture2D(tex,gl_TexCoord[0].xy);
}
