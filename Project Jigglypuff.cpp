/*******************************************************
* ASSIGNMENT 3                                         *
* GROUP NAME:                                          *
* NUR AINA BALQIS BINTI MOHAMAD ZAPARIN - A23CS0151    *
* KRISTINE ELLE BENJAMIN - A23CS0095                   *
* NUR AMIERA ZULAIKHA BINTI HARDI - A23CS0153          *
* SECTION : 01                                         *
*******************************************************/
#include <GL/glut.h>
#include <cmath>
#include <windows.h>
#include <fstream>
#include "imageloader.h"

GLuint jigglypuffPicTexture;
bool showPicture = false;


// Rotation state
float rotX = 0.0f, rotY = 0.0f;
int lastX, lastY;
bool isDragging = false;

float wingFlapAngle = 0.0f;
float wingFlapSpeed = 2.0f;
bool wingFlapDirection = true;

float mouthScale = 1.0f;
bool mouthOpening = true;
float mouthSpeed = 0.05f;

bool isSinging = false; // Jigglypuff singing state
bool beesActive = false; // Bees visibility & animation

// Flight variables
bool isFlying = false;
float flyHeight = 0.0f;
float flySpeed = 0.05f;
float maxFlyHeight = 2.0f;
float jigglypuffRotation = 0.0f;

// Bee data
const int NUM_BEES = 12;
float beeWingAngles[NUM_BEES];      // Wing flap angle per bee
float beeWingSpeeds[NUM_BEES];      // Wing flap speed per bee
float beeOrbitAngles[NUM_BEES];     // Orbit angle around Jigglypuff

bool isBlinking = false;
float blinkTimer = 0.0f;       // Track elapsed time
float blinkDuration = 0.3f;    // How long the blink stays closed
float blinkCooldown = 2.0f;    // Time between blinks


// Initialize bees
void initBees() {
    for (int i = 0; i < NUM_BEES; i++) {
        beeWingAngles[i] = 0.0f;
        beeWingSpeeds[i] = 4.0f + (rand() % 3); // random speed 4~6
        beeOrbitAngles[i] = (360.0f / NUM_BEES) * i; // spread evenly
    }
}

void initLighting() {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawSphere(float radius) {
    glutSolidSphere(radius, 30, 30);
}

void drawEar(float x, float y, float z, bool left) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(left ? -5.0f : 5.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glScalef(0.25f, 0.4f, 0.15f);
    glColor3f(0.95f, 0.6f, 0.7f);
    glutSolidSphere(1.0, 20, 40);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.09f, 0.12f);
    glScalef(0.15f, 0.25f, 0.04f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

void drawEye(bool left) {
    glPushMatrix();
    glTranslatef(left ? -0.3f : 0.3f, 0.2f, 0.55f);

    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.8f);

    if (isBlinking) {
        // Entire eye turns into skin color (blink illusion)
        glColor3f(1.0f, 0.75f, 0.8f);
    }
    else {
        // Normal white eyeball
        glColor3f(1, 1, 1);
    }

    drawSphere(0.26);
    glPopMatrix();

    if (!isBlinking) {
        // Iris (only if not blinking)
        glTranslatef(0, 0.03, 0.07);
        glPushMatrix();
        glScalef(1.0f, 1.0f, 0.8f);
        glColor3f(0.0f, 1.0f, 1.0f);
        drawSphere(0.20);
        glPopMatrix();

        // Highlight
        glTranslatef(0.015f, 0.05f, 0.12);
        glPushMatrix();
        glScalef(1.0f, 1.0f, 0.6f);
        glColor3f(1, 1, 1);
        drawSphere(0.08);
        glPopMatrix();
    }

    glPopMatrix();
}



void drawWing(bool left) {
    glPushMatrix();
    glTranslatef(left ? -0.85f : 0.85f, 0.2f, -0.3f);
    glScalef(left ? -1.0f : 1.0f, 1.0f, 1.0f);
    glRotatef(wingFlapAngle, 1.0f, 0.0f, 0.0f);

    int layers = 3;
    int feathersPerLayer = 3;
    for (int layer = 0; layer < layers; ++layer) {
        float layerYOffset = 0.1f * layer;
        float layerZOffset = -0.05f * layer;

        for (int i = 0; i < feathersPerLayer; ++i) {
            glPushMatrix();
            float xOffset = 0.2f * i;
            float yOffset = layerYOffset + 0.05f * i;
            float rotation = -10 + 5 * i;

            glTranslatef(xOffset, yOffset, layerZOffset);
            glRotatef(rotation, 0, 0, 1);

            float scaleX = 0.35f + 0.05f * layer;
            float scaleY = 0.12f;
            float scaleZ = 0.05f;

            glScalef(scaleX, scaleY, scaleZ);
            glColor3f(0.95f, 0.95f, 0.95f);
            drawSphere(1.0);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

void drawLimb(float x, float y, float z, bool isArm) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.15, isArm ? 0.15 : 0.2, 0.15);
    glColor3f(1.0, 0.75, 0.8);
    drawSphere(1.0);
    glPopMatrix();
}

void drawMouth() {
    glPushMatrix();
    glTranslatef(0.0, -0.18, 0.75);
    glScalef(1.0f, mouthScale, 1.0f);
    glColor3f(0, 0, 0);
    glPointSize(2.5);

    glBegin(GL_POINTS);
    for (float angle = -120.0f; angle <= 120.0f; angle += 1.0f) {
        float rad = angle * 3.14159265f / 180.0f;
        float x = 0.12f * cos(rad);
        float y = 0.13f * sin(rad);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

void drawHairCurl() {
    float angleStep = 0.15f;
    float radius = 0.18f;
    float baseRadius = 0.07f;
    float totalAngle = 7.85f;

    glColor3f(1.0f, 0.75f, 0.8f);

    for (float angle = 0.0f; angle <= totalAngle; angle += angleStep) {
        float x = radius * cosf(angle);
        float y = 0.83f + 0.04f * sinf(angle * 1.5f);
        float z = radius * sinf(angle) + 0.03f * (angle / totalAngle);
        y -= 0.015f * (angle / totalAngle);

        float taper = 1.0f - (angle / totalAngle);
        float currentRadius = baseRadius * taper;

        glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidSphere(currentRadius, 20, 20);
        glPopMatrix();

        radius -= 0.003f;
    }
}

void drawJigglypuff() {
    glPushMatrix();

    // Apply flight transformations
    if (isFlying) {
        glTranslatef(0.0f, flyHeight, 0.0f);
        glRotatef(jigglypuffRotation, 0.0f, 1.0f, 0.0f);
    }

    glColor3f(1.0, 0.75, 0.8);
    drawSphere(0.8);

    drawHairCurl();
    drawEye(true);
    drawEye(false);
    drawMouth();

    drawEar(-0.45, 0.50, 0.0, true);
    drawEar(0.45, 0.50, 0.0, false);

    drawLimb(-0.45, -0.4, 0.55, true);
    drawLimb(0.45, -0.4, 0.55, true);
    drawLimb(-0.3, -0.7, 0.1, false);
    drawLimb(0.3, -0.7, 0.1, false);

    drawWing(true);
    drawWing(false);

    glPopMatrix();
}

void drawBee(float wingAngle) {
    glPushMatrix();

    // ===== Oval Body (yellow) =====
    glPushMatrix();
    glScalef(1.2f, 0.8f, 0.8f); // Oval shape
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glutSolidSphere(0.1, 16, 16);
    glPopMatrix();

    // ===== Black Stripes =====
    for (float zOffset = -0.07f; zOffset <= 0.07f; zOffset += 0.07f) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, zOffset);
        glScalef(1.2f, 0.4f, 0.15f); // Thin stripe, follow body scale
        glColor3f(0.0f, 0.0f, 0.0f); // Black
        glutSolidSphere(0.1, 16, 16);
        glPopMatrix();
    }

    // ===== Elliptical Wings =====
    glPushMatrix();
    glTranslatef(0.0f, 0.12f, 0.0f);
    glRotatef(wingAngle, 1.0f, 0.0f, 0.0f);

    glColor4f(0.8f, 0.8f, 1.0f, 0.7f); // Semi-transparent light blue

    // Left Wing (Ellipse)
    glPushMatrix();
    glTranslatef(-0.08f, 0.0f, 0.0f);
    glScalef(0.15f, 0.05f, 0.08f); // Ellipse scaling
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    // Right Wing (Ellipse)
    glPushMatrix();
    glTranslatef(0.08f, 0.0f, 0.0f);
    glScalef(0.15f, 0.05f, 0.08f); // Ellipse scaling
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    glPopMatrix(); // End first wing rotation

    // Second pair of wings
    glPushMatrix();
    glTranslatef(0.0f, 0.12f, 0.0f);
    glRotatef(-wingAngle, 1.0f, 0.0f, 0.0f);

    glColor4f(0.8f, 0.8f, 1.0f, 0.7f);

    // Left Wing (Ellipse)
    glPushMatrix();
    glTranslatef(-0.08f, 0.0f, 0.0f);
    glScalef(0.15f, 0.05f, 0.08f);
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    // Right Wing (Ellipse)
    glPushMatrix();
    glTranslatef(0.08f, 0.0f, 0.0f);
    glScalef(0.15f, 0.05f, 0.08f);
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();

    glPopMatrix(); // End second wing rotation

    glPopMatrix(); // End bee
}

void drawBees() {
    if (!beesActive) return;

    float orbitRadius = 2.0f;
    for (int i = 0; i < NUM_BEES; i++) {
        // Calculate bee position in orbit
        float angleRad = beeOrbitAngles[i] * 3.14159265f / 180.0f;
        float x = orbitRadius * cosf(angleRad);
        float z = orbitRadius * sinf(angleRad);
        float y = 0.5f + flyHeight + 0.1f * sinf(beeOrbitAngles[i] * 5.0f * 3.14159265f / 180.0f); // slight up/down

        glPushMatrix();
        glTranslatef(x, y, z);

        // Rotate bees slowly to face center
        glRotatef(-beeOrbitAngles[i], 0.0f, 1.0f, 0.0f);

        // Animate wings with current bee wing flap angle
        drawBee(beeWingAngles[i]);
        glPopMatrix();
    }
}



void update(int value) {
    // Wing flapping logic for Jigglypuff only when flying
    if (isFlying) {
        if (wingFlapDirection) {
            wingFlapAngle += wingFlapSpeed;
            if (wingFlapAngle > 15.0f) wingFlapDirection = false;
        }
        else {
            wingFlapAngle -= wingFlapSpeed;
            if (wingFlapAngle < -15.0f) wingFlapDirection = true;
        }

        // Flight movement synchronized with wing flaps
        if (wingFlapDirection) {
            flyHeight += flySpeed * 0.1f;
            if (flyHeight > maxFlyHeight) flyHeight = maxFlyHeight;
        }
        else {
            flyHeight -= flySpeed * 0.05f;
            if (flyHeight < 0.0f) flyHeight = 0.0f;
        }

        // Slowly rotate while flying
        jigglypuffRotation += 0.5f;
        if (jigglypuffRotation > 360.0f) jigglypuffRotation -= 360.0f;
    }
    else {
        wingFlapAngle = 0.0f; // Reset wing angle when not flying
    }

    // Mouth animation only when singing
    if (isSinging) {
        if (mouthOpening) {
            mouthScale += mouthSpeed;
            if (mouthScale >= 1.5f) mouthOpening = false;
        }
        else {
            mouthScale -= mouthSpeed;
            if (mouthScale <= 1.0f) mouthOpening = true;
        }
    }
    else {
        mouthScale = 1.0f;
    }

    // Update bees animation
    if (beesActive) {
        for (int i = 0; i < NUM_BEES; i++) {
            beeWingAngles[i] += beeWingSpeeds[i];
            if (beeWingAngles[i] > 45.0f) beeWingAngles[i] = 0.0f;

            beeOrbitAngles[i] += 1.5f;
            if (beeOrbitAngles[i] >= 360.0f) beeOrbitAngles[i] -= 360.0f;
        }
    }

    // Eye blinking animation
    // Auto-blinking logic
    blinkTimer += 0.016f; // Approx. 60 FPS

    if (!isBlinking && blinkTimer >= blinkCooldown) {
        isBlinking = true;
        blinkTimer = 0.0f;
    }
    else if (isBlinking && blinkTimer >= blinkDuration) {
        isBlinking = false;
        blinkTimer = 0.0f;
    }


    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void renderBitmap(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void displayInstructions() {
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 800); // 2D orthographic projection

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float textX = 10.0f;
    float textY = 780.0f;
    float lineHeight = 20.0f;
    char buf[256];

    glColor3f(0.0f, 0.0f, 0.0f); // Black - high contrast
    sprintf_s(buf, "Instructions:");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(0.1f, 0.1f, 0.1f); // Dark grey

    sprintf_s(buf, "Press 'A': Show/Hide Bees Swarm");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    sprintf_s(buf, "Press 'F': Toggle Flight Mode");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    sprintf_s(buf, "Mouse Drag: Rotate Scene");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    sprintf_s(buf, "Press 'Esc': Exit");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(0.5f, 0.0f, 0.5f); // Purple - visible and nice
    sprintf_s(buf, "Enjoy the animation and music!");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(0.8f, 0.2f, 0.0f); // Dark orange
    sprintf_s(buf, "Audio: Jigglypuff Song");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(0.2f, 0.4f, 0.8f); // Dark blue
    sprintf_s(buf, "Press Spacebar: Play/Stop Singing");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(0.2f, 0.6f, 0.0f); // Dark green
    sprintf_s(buf, "Press 'A' to bring out the bees!");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glColor3f(1.0f, 0.2f, 0.6f);
    sprintf_s(buf, "Press 'P' to display the strength and weakness");
    renderBitmap(textX, textY, GLUT_BITMAP_8_BY_13, buf); textY -= lineHeight;

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void drawLand() {
    glPushMatrix();
    glDisable(GL_LIGHTING); // Optional: if you want flat color

    glColor3f(0.3f, 0.8f, 0.3f); // Grass green
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -1.0f, -10.0f);
    glVertex3f(-10.0f, -1.0f, 10.0f);
    glVertex3f(10.0f, -1.0f, 10.0f);
    glVertex3f(10.0f, -1.0f, -10.0f);
    glEnd();

    glEnable(GL_LIGHTING); // Re-enable lighting
    glPopMatrix();
}

void drawJigglypuffPicture() {
    if (!showPicture) return;

    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, jigglypuffPicTexture);
    glColor3f(1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 800);  // 2D space

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(544, 500);
    glTexCoord2f(1, 0); glVertex2f(800, 500);
    glTexCoord2f(1, 1); glVertex2f(800, 800);
    glTexCoord2f(0, 1); glVertex2f(544, 800);
    glEnd();


    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}



void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 8.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);

    drawLand();
    drawJigglypuff();
    drawBees();
    displayInstructions();
    drawJigglypuffPicture();
    glutSwapBuffers();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastX = x;
            lastY = y;
        }
        else {
            isDragging = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        rotY += (x - lastX) * 0.5f;
        rotX += (y - lastY) * 0.5f;
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        beesActive = !beesActive;
        if (beesActive) {
            initBees();
        }
        break;
    case 'f':
    case 'F':
        isFlying = !isFlying;
        if (!isFlying) {
            // Slowly descend when stopping flight
            while (flyHeight > 0.0f) {
                flyHeight -= 0.05f;
                if (flyHeight < 0.0f) flyHeight = 0.0f;
                glutPostRedisplay();
            }
        }
        break;
    case ' ':  // Spacebar to toggle singing
        isSinging = !isSinging;
        if (isSinging) {
            PlaySound(TEXT("C:\\zmisc\\jigglypuf.wav"), NULL, SND_ASYNC | SND_LOOP);
        }
        else {
            PlaySound(NULL, 0, 0);
        }
        break;

    case 'p':
    case 'P':
        showPicture = !showPicture;
        break;

    case 27:  // Escape key
        exit(0);
        break;

    }

}

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        image->width, image->height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureId;
}

void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Image* img = loadBMP("C:\\zmisc\\JigglyPuff.bmp");
    jigglypuffPicTexture = loadTexture(img);
    delete img;

}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Jigglypuff");

    init();
    initLighting();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}