//  Author: John Salame
//  Per Pixel Lighting shader with texture
//  Modified from Example 25
#version 120

const int numLights = 2;
varying vec4 P;
varying vec3 Normal;
uniform sampler2D tex;

void main()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
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
   myColor = gl_Color * (0.2 + 0.8*myColor); // have a baseline of 0.2*glColor and mix in 0.8*(lighting and texture)

   //  Apply texture
   gl_FragColor = myColor * texture2D(tex,gl_TexCoord[0].xy);
}
