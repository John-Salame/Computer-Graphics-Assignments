//  Randomly turn pixels white on the screen
//  Author: John Salame
#version 120

uniform float pixChance = 0.01;
uniform float t; // time, use as a seed

// gold noise function taken from https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
// Gold Noise ©2015 dcerisano@standard3d.com
// - based on the Golden Ratio
// - uniform normalized distribution
// - fastest static noise generator function (also runs at low precision)
// - use with indicated fractional seeding method.

float PHI = 1.61803398874989484820459;  // PHI = Golden Ratio

float gold_noise(in vec2 xy, in float seed) {
    return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

void main()
{
    float randVal = gold_noise(gl_FragCoord.xy, t); // hopefully fract is always positive
    if(randVal > pixChance) {
        discard;
    }
    gl_FragColor = gl_Color;
}
