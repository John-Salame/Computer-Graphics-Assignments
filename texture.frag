//  Set the fragment color
//  Taken from Example 25
#version 120

uniform sampler2D tex;

void main()
{
   gl_FragColor = gl_Color * texture2D(tex,gl_TexCoord[0].xy);
}
