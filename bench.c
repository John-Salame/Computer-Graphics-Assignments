/**
 * Author: John Salame
 * Date: 12/29/22
 * CSCI 5229 Computer Graphics
 *
 * A park bench using per-pixel lighting
 */
#include "objects.h"

/**
 * Create a fillet at the location constrained by side and top. Use 30 degree angles.
 * The function will translate the world coordinates to 
 * @param right: use value 1.0 if the fillet faces right (CCW is down and toward -z)
 *               use value -1.0 if the fillet faces left (CCW is down and toward +z)
 * @param radius: radius of the fillet
 * @param side: the x value where the fillet ends
 * @param top: the y value where the fillet ends
 */
void createFillet(float right, float radius, float side, float top,  unsigned int programName, float texS1, float texS2) {
    // enable the vertex attribute for Tangent vector
    unsigned int tangentIndex = glGetAttribLocation(programName, "InTangent");
    unsigned int bitangentDirIndex = glGetAttribLocation(programName, "BitangentDir");
    glEnableVertexAttribArray(tangentIndex);
    glEnableVertexAttribArray(bitangentDirIndex);
    ErrCheck("bench fillet enable vertex attributes");

    const float x[4] = {0.0, 0.5, 0.866, 1.0};
    const float y[4] = {1.0, 0.866, 0.5, 0.0};
    float firstZ = 0.5*right;
    float secondZ = -0.5*right;
    float texIncrement = right * (texS2 - texS1) / 3.0; // span of texture coordinates per quad
    // note that on the right fillet, the texture coordinates ascend as we descend the fillet;
    // however, the left fillet has texture coordinates ascending as we ascend the fillet
    float texStart = texS1;
    if(right == -1.0) {
        texStart = texS2;
    }
    float texEnd = texStart + texIncrement;

    // deal with the weirdness of the left fillet
    float firstTexT = 0.0;
    float secondTexT = 1.0;
    if(right == -1.0) {
        firstTexT = 1.0;
        secondTexT = 0.0;
    }
    glPushMatrix();
    // scale so that the fillet can be made with a unit circle, then translate
    glTranslatef(side-right*radius, top-radius, 0.0); // translate to the center of the fillet
    glScalef(right*radius, radius, 1.0); // scaling will negate the x values if the fillet faces left (right == -1)
    // make 3 planes starting from the top of the fillet
    glNormal3f(0.0, 1.0, 0.0);
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, right*1.0); // bitangent = normal x tangent
    glBegin(GL_QUADS);
    for(int i = 0; i < 3; i++) {
        glTexCoord2f(texStart, secondTexT); glVertex3f(x[i], y[i], secondZ);
        glTexCoord2f(texStart, firstTexT); glVertex3f(x[i], y[i], firstZ);
        // lower y values have a new normal and tanent vector
        // glNormal3f(y[i] - y[i+1], x[i+1] - x[i], 0.0); // normal is (-dy, dx, 0.0)
        glNormal3f(x[i+1], y[i+1], 0.0); // normal on a unit circle is just the same as the point
        glVertexAttrib3f(tangentIndex, y[i+1], -x[i+1], 0.0); // tangent has (y, -x) from the normal vector
        glTexCoord2f(texEnd, firstTexT); glVertex3f(x[i+1], y[i+1], firstZ);
        glTexCoord2f(texEnd, secondTexT); glVertex3f(x[i+1], y[i+1], secondZ);
        texStart = texEnd;
        texEnd += texIncrement;
    }
    glEnd();

    // draw the front part of the butt slat where the fillet is;
    // both texture dimensions span [0, 1]
    float texX = (1 - right) / 2.0; // if we have a right fillet, center x=0; else, center x=1
    float texY = top - (radius / top); // 1.0 minus the proportion of slat height taken up by the fillet
    glBegin(GL_TRIANGLE_FAN);
    glVertexAttrib1f(bitangentDirIndex, right); // bitangent = normal x tangent if fillet is a right fillet
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glTexCoord2f(texX, texY);
    // printf("fillet texX %f texY %f", texX, texY);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.5);
    // draw clockwise
    if(right == 1.0) {
        for(int i = 3; i >= 0; i--) {
            glTexCoord2f(texX+right*Sin(30*i), texY+Cos(30*i));
            glVertex3f(x[i], y[i], 0.5);
        }
    }
    else {
        for(int i = 0; i < 4; i++) {
            glTexCoord2f(texX+right*Sin(30*i), texY+Cos(30*i));
            glVertex3f(x[i], y[i], 0.5);
        }
    }
    glEnd();
    glPopMatrix();
    // do the error check
    char errStr[100];
    snprintf(errStr, 100, "fillet right %f side %f top %f", right, side, top);
    errStr[99] = 0;
    ErrCheck(errStr);
}

/**
 * Draw the butt slat starting from the bottom-left point
 * x goes from 0.0 to 2*filletRad + flatWidth
 * y goes from 0.0 to thickness
 * z goes from -0.5 to 0.5
 * @param thickness: how tall the slat is
 * @param filletRad: width and height of fillet
 * @param flatWidth: how wide (x direction) the flat part at the top of the butt slat is
 * @param texS1: where to start wrapping the texture (s (x) value)
 * @param texS2: where to finish wrapping the texture (s (x) value)
 */
void buttSlat(float thickness, float filletRad, float flatWidth, unsigned int programName, float texS1, float texS2) {
    // enable the vertex attribute for Tangent vector
    unsigned int tangentIndex = glGetAttribLocation(programName, "InTangent");
    unsigned int bitangentDirIndex = glGetAttribLocation(programName, "BitangentDir");
    glEnableVertexAttribArray(tangentIndex);
    glEnableVertexAttribArray(bitangentDirIndex);
    ErrCheck("bench butt slat enable vertex attributes");

    // set most of the globals
    const float incrementFraction = 0.125; // 1/8 since that roughly lines up with how we can split the model into sides, fillets, and flat top
    const float benchTop = thickness;
    const float flatStart = filletRad; // x value of flat part
    const float flatEnd = flatStart + flatWidth;
    const float width = 2*filletRad + flatWidth;
    float filletBottom = thickness - filletRad; // how high up the side quads should go
    const float texSpan = texS2 - texS1;
    const float texIncrement = texSpan / 8.0; // sides = 1 texIncrement, fillets = 1 texIncrement, top = 4 texIncrement
    float texStart = texS1; // start of the current quad's texture
    float texEnd = texS2;

    // make the rectangular parts of the butt slat
    glBegin(GL_QUADS);
    // bottom (spans the whole texture texS1 (0.0) to texS2 (width))
    // s aligns with x and t aligns with -z
    texStart = texS1;
    texEnd = texS1 + texSpan;
    glNormal3f(0.0, -1.0, 0.0);
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, 0.0, 0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(0.0, 0.0, -0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(width, 0.0, -0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, 0.0, 0.5);
    // left quad
    // takes up 1 texIncrement of space
    texStart = texS1;
    texEnd = texStart + texIncrement;
    glNormal3f(-1.0, 0.0, 0.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texEnd, 1.0); glVertex3f(0.0, filletBottom, -0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(0.0, 0.0, -0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, 0.0, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(0.0, filletBottom, 0.5);
    // top quad (flat width distance in x direction)
    // s aligns with x and t aligns with -z
    texStart = texS1 + 2*texIncrement;
    texEnd = texStart + 4*texIncrement;
    glNormal3f(0.0, 1.0, 0.0);
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texStart, 1.0); glVertex3f(flatStart, benchTop, -0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(flatStart, benchTop, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(flatEnd, benchTop, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(flatEnd, benchTop, -0.5);
    // right quad
    texStart = texS1 + 7*texIncrement;
    texEnd = texStart + texIncrement;
    glNormal3f(1.0, 0.0, 0.0);
    glVertexAttrib3f(tangentIndex, 0.0, -1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texStart, 0.0); glVertex3f(width, filletBottom, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, 0.0, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(width, 0.0, -0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(width, filletBottom, -0.5);

    // front panel part 1 (lower part)
    texStart = 0.0;
    texEnd = 1.0;
    float filletBottomTex = 6*incrementFraction; // 6 increments of 1/8 since that is the unit this model is made in
    glNormal3f(0.0, 0.0, 1.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
    glTexCoord2f(texStart, filletBottomTex); glVertex3f(0.0, filletBottom, 0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, 0.0, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, 0.0, 0.5);
    glTexCoord2f(texEnd, filletBottomTex); glVertex3f(width, filletBottom, 0.5);
    //front panel part 2 (upper part)
    texStart = 2*incrementFraction;
    texEnd = 1.0-2*incrementFraction;
    glNormal3f(0.0, 0.0, 1.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
    glTexCoord2f(texStart, 1.0); glVertex3f(flatStart, benchTop, 0.5);
    glTexCoord2f(texStart, filletBottomTex); glVertex3f(flatStart, filletBottom, 0.5);
    glTexCoord2f(texEnd, filletBottomTex); glVertex3f(flatEnd, filletBottom, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(flatEnd, benchTop, 0.5);
    glEnd();
    // make the right fillet and then the left fillet
    // params: right (1.0 or -1.0), radius, side, top
    texStart = texS1 + 6*texIncrement;
    texEnd = texStart + texIncrement; // should be equal to texS2
    createFillet(1.0, filletRad, width, benchTop, programName, texStart, texEnd); // right fillet
    texStart = texS1 + texIncrement;
    texEnd = texStart + texIncrement;
    createFillet(-1.0, filletRad, 0.0, benchTop, programName, texStart, texEnd); // left fillet
}

void bench(float width, float height, float depth, unsigned int texName, unsigned int normalMapName, unsigned int programName) {
    // prepare textures
    glUseProgram(programName);
    unsigned int texLoc = glGetUniformLocation(programName, "tex");
    unsigned int normalLoc = glGetUniformLocation(programName, "normalMap");
    // bind the uniform samplers to texture units (0 for decal, 1 for normal map)
    ErrCheck("bench getting uniforms");
    glUniform1i(texLoc, 0);
    glUniform1i(normalLoc, 1);
    ErrCheck("bench setting uniforms");
    // texture settings
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, normalMapName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    ErrCheck("bench setting textures");

    // prepare most of the variables
    const float black[] = {0.0, 0.0, 0.0, 1.0};
    const float darkGray[] = {0.3, 0.3, 0.3, 1.0};
    const float lightGray[] =  {0.5, 0.3, 0.6, 1.0};
    const float darkBrown[] = {0.6, 0.4, 0.3, 1.0};
    const float thickness = 0.05; // material thickness is 10% of the height
    const float benchBottom = 0.4;
    const float benchTop = benchBottom + thickness;
    // the space variable needs to be calculated for an exact fit.
    // butt slats (for lack of a better term) have this ratio: ? space, 0.1 fillet, 0.2 flat, 0.1 fillet, ? space
    const int numButtSlats = 20;
    const float filletRad = 0.01;
    const float flatWidth = 0.02;
    const float buttSlatWidth = 2*filletRad + flatWidth;
    
    // disable face culling when drawing the bench
    GLboolean useCulling = glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    // set color and material properties for metal back of bench
    glColor4fv(darkGray);
    glMaterialfv(GL_FRONT, GL_AMBIENT, darkGray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, darkGray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, darkGray);

    // save the matrix and scale the bench
    glPushMatrix();
    glScalef(width, height, depth);

    // front
    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 1.0, -0.5);
    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(0.5, 0.0, -0.5);
    glVertex3f(0.5, 1.0, -0.5);
    glEnd();

    // glUseProgram(0);

    // set color and material properties for wooden butt slats
    glColor4fv(darkBrown); // without the color, the snow mound is white. It seems like the materials don't do anything at all to the color.
    glMaterialfv(GL_FRONT, GL_AMBIENT, darkBrown); // snow looks blue at night
    glMaterialfv(GL_FRONT, GL_DIFFUSE, darkBrown);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black); // no specular for wood

    // calculate space (one space exists on each side of butt slat; distance between butt slats is 2*space)
    // we have 2*space per butt slat
    const float buttSlatArea = numButtSlats * buttSlatWidth;
    if(buttSlatArea > 1.0) {
        Fatal("too many butt slats! %d butt slats taking up %f length", numButtSlats, buttSlatArea);
    }
    const float space = (1.0 - buttSlatArea) / (2.0 * numButtSlats);
    const float slatOffset = buttSlatWidth + 2*space;

    // draw the butt slats
    float buttSlatStart = -0.5;
    for(int i = 0; i < numButtSlats; i++) {
        float texS1 = buttSlatStart;
        glPushMatrix();
        glTranslatef(buttSlatStart+space, benchBottom, 0.0);
        buttSlat(thickness, filletRad, flatWidth, programName, texS1, texS1 + slatOffset);
        glPopMatrix();
        buttSlatStart += slatOffset;
    }
    
    // reset face culling setting
    if(useCulling) {
        glEnable(GL_CULL_FACE);
    }
    glPopMatrix();
    ErrCheck("bench");
}
