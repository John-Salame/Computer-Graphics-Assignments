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
 * @param programName: shader program to use (should have the capability for normal maps or ErrCheck will print errors)
 * @param texS1: starting texture s (x) coordinate
 * @param texS2: final texture s (x) coordinate
 * @param frontTexS1: first texture coordinate to use for front face of fillet (triangle fan).
 * @param frontTexS2: last texture coordinate to use for front face of fillet (triangle fan).
 * @constraint: frontTexS1 < frontTexS2
 */
void createFillet(float right, float radius, float side, float top,  unsigned int programName, float texS1, float texS2, float frontTexS1, float frontTexS2) {
    glPushMatrix();
    // scale so that the fillet can be made with a unit circle, then translate
    glTranslatef(side-right*radius, top-radius, 0.0); // translate to the center of the fillet
    glScalef(radius, radius, 1.0); // scaling will negate the x values if the fillet faces left (right == -1)
    
    // enable the vertex attribute for Tangent vector
    unsigned int tangentIndex = glGetAttribLocation(programName, "InTangent");
    unsigned int bitangentDirIndex = glGetAttribLocation(programName, "BitangentDir");
    glEnableVertexAttribArray(tangentIndex);
    glEnableVertexAttribArray(bitangentDirIndex);
    ErrCheck("bench fillet enable vertex attributes");

    float x[4] = {0.0, 0.5, 0.866, 1.0}; // this is not const because it changes to negative values if we draw a left fillet
    const float y[4] = {1.0, 0.866, 0.5, 0.0};
    float firstZ = 0.5;
    float secondZ = -0.5;
    // *TexT: t (y) component of texture associated with firstZ and secondZ
    float firstTexT = 0.0;
    float secondTexT = 1.0;
    // span of texture coordinates per quad
    float texIncrement = (texS2 - texS1) / 3.0;
    // note that on the right fillet, the texture coordinates ascend as we descend the fillet;
    // however, the left fillet has texture coordinates ascending as we ascend the fillet
    float texStart = texS1;

    // deal with the weirdness of the left fillet;
    // textures are drawn from texS2 to texS1, order of z values drawn is flipped, and x values are negated.
    if(right == -1.0) {
        texStart = texS2;
        texIncrement = -1 * texIncrement; // decrease texture coordinates as you descend the fillet
        // swap first z and second z so we still draw the shape counter-clockwise
        float tmp = firstZ;
        firstZ = secondZ;
        secondZ = tmp;
        // swap firstTexT and secondTexT
        tmp = firstTexT;
        firstTexT = secondTexT;
        secondTexT = tmp;
        // points should be in negative x direction from the center
        for(int i = 0; i < 4; i++) {
            x[i] = -1 * x[i];
        }
    }
    float texEnd = texStart + texIncrement;

    // make 3 planes starting from the top of the fillet
    glNormal3f(0.0, 1.0, 0.0);
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
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
    // first, get the coordinates of the center of the fillet on the texture map
    float texFilletRad = (frontTexS2 - frontTexS1);
    float texCenterY = 1.0 - texFilletRad;
    float texCenterX = frontTexS1;
    if(right == -1.0) {
        texCenterX = frontTexS2;
    }
    glBegin(GL_TRIANGLE_FAN);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(texCenterX, texCenterY);
    glVertex3f(0.0, 0.0, 0.5);
    // draw counter-clockwise
    if(right == 1.0) {
        for(int i = 3; i >= 0; i--) {
            glTexCoord2f(texCenterX+texFilletRad*Sin(30*i), texCenterY+texFilletRad*Cos(30*i));
            glVertex3f(x[i], y[i], 0.5);
        }
    }
    else {
        for(int i = 0; i < 4; i++) {
            glTexCoord2f(texCenterX-texFilletRad*Sin(30*i), texCenterY+texFilletRad*Cos(30*i));
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
 * y goes from seatBottom to seatTop (in local coordinates, this is 0.0 to thickness)
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
    const float seatBottom = 0.0;
    const float seatTop = seatBottom + thickness;
    float filletBottom = seatTop - filletRad; // how high up the side quads should go
    const float width = 2*filletRad + flatWidth;
    const float firstX = 0.0;
    const float lastX = width;
    const float flatStart = firstX + filletRad; // x value of flat part
    const float flatEnd = flatStart + flatWidth;
    const float texSpan = texS2 - texS1;
    const float texIncrement = texSpan * incrementFraction; // sides = 1 texIncrement, fillets = 1 texIncrement, top = 4 texIncrement
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
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, seatBottom, 0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(0.0, seatBottom, -0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(width, seatBottom, -0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, seatBottom, 0.5);
    // left quad
    // takes up 1 texIncrement of space
    texStart = texS1;
    texEnd = texStart + texIncrement;
    glNormal3f(-1.0, 0.0, 0.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texEnd, 1.0); glVertex3f(0.0, filletBottom, -0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(0.0, seatBottom, -0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, seatBottom, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(0.0, filletBottom, 0.5);
    // top quad (flat width distance in x direction)
    // s aligns with x and t aligns with -z
    texStart = texS1 + 2*texIncrement;
    texEnd = texStart + 4*texIncrement;
    glNormal3f(0.0, 1.0, 0.0);
    glVertexAttrib3f(tangentIndex, 1.0, 0.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texStart, 1.0); glVertex3f(flatStart, seatTop, -0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(flatStart, seatTop, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(flatEnd, seatTop, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(flatEnd, seatTop, -0.5);
    // right quad
    texStart = texS1 + 7*texIncrement;
    texEnd = texStart + texIncrement;
    glNormal3f(1.0, 0.0, 0.0);
    glVertexAttrib3f(tangentIndex, 0.0, -1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, 1.0); // bitangent = normal x tangent
    glTexCoord2f(texStart, 0.0); glVertex3f(width, filletBottom, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, seatBottom, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(width, seatBottom, -0.5);
    glTexCoord2f(texStart, 1.0); glVertex3f(width, filletBottom, -0.5);

    // front panel part 1 (lower part)
    texStart = 0.0;
    texEnd = 1.0;
    float filletBottomTex = 6*incrementFraction; // 6 increments of 1/8 since that is the unit this model is made in
    glNormal3f(0.0, 0.0, 1.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
    glTexCoord2f(texStart, filletBottomTex); glVertex3f(0.0, filletBottom, 0.5);
    glTexCoord2f(texStart, 0.0); glVertex3f(0.0, seatBottom, 0.5);
    glTexCoord2f(texEnd, 0.0); glVertex3f(width, seatBottom, 0.5);
    glTexCoord2f(texEnd, filletBottomTex); glVertex3f(width, filletBottom, 0.5);
    //front panel part 2 (upper part)
    texStart = 2*incrementFraction;
    texEnd = 1.0-2*incrementFraction;
    glNormal3f(0.0, 0.0, 1.0);
    glVertexAttrib3f(tangentIndex, 0.0, 1.0, 0.0);
    glVertexAttrib1f(bitangentDirIndex, -1.0); // bitangent = tangent x normal
    glTexCoord2f(texStart, 1.0); glVertex3f(flatStart, seatTop, 0.5);
    glTexCoord2f(texStart, filletBottomTex); glVertex3f(flatStart, filletBottom, 0.5);
    glTexCoord2f(texEnd, filletBottomTex); glVertex3f(flatEnd, filletBottom, 0.5);
    glTexCoord2f(texEnd, 1.0); glVertex3f(flatEnd, seatTop, 0.5);
    glEnd();
    // make the right fillet and then the left fillet
    // params: right (1.0 or -1.0), radius, side, top
    texStart = texS1 + 6*texIncrement;
    texEnd = texStart + texIncrement; // should be equal to texS2
    createFillet(1.0, filletRad, lastX, seatTop, programName, texStart, texEnd, 6*incrementFraction, 1.0); // right fillet
    // left fillet
    texStart = texS1 + texIncrement;
    texEnd = texStart + texIncrement;
    createFillet(-1.0, filletRad, firstX, seatTop, programName, texStart, texEnd, 0.0, 2*incrementFraction); // left fillet
}


/**
 * Make a bench with the center (y at bottom of the legs, x and z = center of seat).
 * texName and normalMapName correspond to the wood texture used in the seat and back rest.
 */
void bench(float width, float height, float depth, unsigned int texName, unsigned int normalMapName, unsigned int programName) {
    glUseProgram(programName);
    // save the matrix and scale the entire bench
    glPushMatrix();
    glScalef(width, height, depth);
    
    // record whether face culling is enabled, since some parts of the bench disable culling
    GLboolean useCulling = glIsEnabled(GL_CULL_FACE);
    
    // prepare textures
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
    const float woodColor[] = {0.6, 0.4, 0.3, 1.0};
    const float thickness = 0.05; // material thickness, on proportion to total bench height
    const float seatBottom = 0.4;
    const float seatTop = seatBottom + thickness;
    // the space variable needs to be calculated for an exact fit.
    // butt slats (for lack of a better term) have this ratio: ? space, 0.1 fillet, 0.2 flat, 0.1 fillet, ? space
    const int numButtSlats = 20;
    const float filletRad = 0.01;
    const float flatWidth = 0.02;
    const float buttSlatWidth = 2*filletRad + flatWidth;
    // variables for metal bars
    const float metalDx = 0.5 * thickness; // how far the center of the metal rod is from the wood
    const float metalRad = 1.414 * metalDx; // sqrt(2)*metalDx; result: wood contacts metal rod at 45 degree mark
    // variables for back rest
    const float backRestAngle = 75.0;
    const float backRestLength = 1.0 - seatBottom - metalDx - metalRad*Sin(135-backRestAngle);

    // variables for bench legs
    float legAngle = 90 + 2*(90-backRestAngle); // less steep than back rest angle; start with angle for back legs
    const float legLength = (seatBottom + metalDx + metalDx*Sin(-45-legAngle)) / Sin(legAngle);
    // calculation for leg length, using legAngle as the angle down from the x-z plane (for back legs first)
    // the legs are smaller radius than the connectors and same diameter as the thickness of the "butt slat" seat planks
    // the length of the legs needs to be such that the highest part of the leg touches the ground
    // legLength*Sin(legAngle) = seatBottom + metalDx + metalDx*Sin(-45-legAngle)
    // legLength = (seatBottom + metalDx + metalDx*Sin(-45-legAngle)) / Sin(legAngle)

    // set color and material properties for wooden butt slats
    glColor4fv(woodColor);
    glMaterialfv(GL_FRONT, GL_AMBIENT, woodColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, woodColor);
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
        // draw butt slat on seat
        glPushMatrix();
        glTranslatef(buttSlatStart+space, seatBottom, 0.0);
        buttSlat(thickness, filletRad, flatWidth, programName, texS1, texS1 + slatOffset);
        glPopMatrix();
        // draw "butt slat" on back rest
        // Remember to read transformations from bottom up
        glPushMatrix();
        glTranslatef(buttSlatStart+space, seatBottom + metalDx, -0.5-metalDx); // position the metal rod where it belongs
        glRotatef(backRestAngle, 1.0, 0.0, 0.0); // rotate around the metal pole connecting the seat to the back rest
        glTranslatef(0.0, -metalDx, -0.5*backRestLength-metalDx); // position butt slat on circle edge before rotation
        glScalef(1.0, 1.0, backRestLength); // fit the slat within the height limit, height=1.0 before bench universal scaling
        // textures on the back rest are stretched out and the scratches are shifted
        buttSlat(thickness, filletRad, flatWidth, programName, 0.5*(texS1+0.7), 0.5*(texS1+0.7+slatOffset));
        glPopMatrix();
        buttSlatStart += slatOffset;
    }
    ErrCheck("Bench butt slats and back rest");

    // Draw the metal rods (or candy cane rods) - connector and top of back rest
    // disable face culling since we can see in side the rods
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glTranslatef(-0.5, seatBottom + metalDx, -0.5-metalDx); // position the metal rod where it belongs
    glPushMatrix();
    // connector rod
    glRotatef(90, 0.0, 1.0, 0.0);
    RedStripedCylinderWall(30, metalRad, 1.0, programName);
    glPopMatrix();
    ErrCheck("Bench connector");
    // top metal rod
    glPushMatrix();
    glRotatef(backRestAngle, 1.0, 0.0, 0.0); // rotate around the metal pole connecting the seat to the back rest
    glTranslatef(0.0, 0.0, -backRestLength - 2*metalDx); // position the second metal rod (at the top of the back rest)
    glRotatef(90, 0.0, 1.0, 0.0);
    RedStripedCylinderWall(30, metalRad, 1.0, programName);
    glPopMatrix();
    ErrCheck("Bench upper rod");
    // draw back legs
    glPushMatrix();
    glRotatef(legAngle, 1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, legLength);
    // left front leg
    glPushMatrix();
    glTranslatef(metalDx, 0.0, 0.0);
    RedStripedCylinderWall(30, metalDx, 1.0, programName);
    glPopMatrix();
    // right front leg
    glPushMatrix();
    glTranslatef(1.0 - metalDx, 0.0, 0.0);
    RedStripedCylinderWall(30, metalDx, 1.0, programName);
    glPopMatrix();
    glPopMatrix();
    ErrCheck("Bench back legs");
    glPopMatrix();
    ErrCheck("Bench connectors and back legs");

    // draw front legs and connector for this push/pop segment
    // Note: leg angle should be less than 90 degrees down from z-axis now; calculate the correct angle
    legAngle = 180 - legAngle;
    glPushMatrix();
    glTranslatef(-0.5, seatBottom, 0.5+metalDx); // y and z of front connector
    // draw front connector (where front legs come from)
    glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    RedStripedCylinderWall(30, metalRad, 1.0, programName);
    glPopMatrix();
    // draw front legs
    glRotatef(legAngle, 1.0, 0.0, 0.0);
    glScalef(1.0, 1.0, legLength);
    // left front leg
    glPushMatrix();
    glTranslatef(metalDx, 0.0, 0.0);
    RedStripedCylinderWall(30, metalDx, 1.0, programName);
    glPopMatrix();
    // right front leg
    glPushMatrix();
    glTranslatef(1.0 - metalDx, 0.0, 0.0);
    RedStripedCylinderWall(30, metalDx, 1.0, programName);
    glPopMatrix();
    glPopMatrix();
    ErrCheck("Bench front legs");
    
    // reset face culling setting
    if(useCulling) {
        glEnable(GL_CULL_FACE);
    }
    glPopMatrix();
    ErrCheck("bench");
}
