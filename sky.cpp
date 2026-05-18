#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1416f

// ANIMATION STATE
float t = 0.0f;
float jetUSAx = 50.0f;
float cloudShift = 0.0f;
float starTwinkle = 0.0f;
float satX = 180.0f;
float satBaseY = 420.0f;
float satPhase = 0.0f;

// Intro states
enum SceneState {
    INTRO_EXPLOSION,
    INTRO_TEXT,
    NORMAL
};

SceneState sceneState = INTRO_EXPLOSION;
float introT = 0.0f;

// Sky stage
int skyStage = 0;

// HELPER FUNCTIONS
void drawCircle(float cx, float cy, float radius) {
    int segments = 48;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * PI * i / segments;
        glVertex2f(cx + radius * cos(a), cy + radius * sin(a));
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry) {
    int segments = 48;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float a = 2.0f * PI * i / segments;
        glVertex2f(cx + rx * cos(a), cy + ry * sin(a));
    }
    glEnd();
}

void drawSoftCircle(float cx, float cy, float radius,
                    float r, float g, float b, float innerA, float outerA) {
    int segments = 40;
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLE_FAN);
        glColor4f(r, g, b, innerA); glVertex2f(cx, cy);
        glColor4f(r, g, b, outerA);
        for (int i = 0; i <= segments; i++) {
            float a = 2.0f * PI * i / segments;
            glVertex2f(cx + radius * cos(a), cy + radius * sin(a));
        }
    glEnd();
    glDisable(GL_BLEND);
}

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1,y1); glVertex2f(x2,y1);
    glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
}

void drawGradRect(float x1, float y1, float x2, float y2,
                  float r1, float g1, float b1,
                  float r2, float g2, float b2) {
    glBegin(GL_QUADS);
        glColor3f(r1,g1,b1); glVertex2f(x1,y1);
        glColor3f(r1,g1,b1); glVertex2f(x2,y1);
        glColor3f(r2,g2,b2); glVertex2f(x2,y2);
        glColor3f(r2,g2,b2); glVertex2f(x1,y2);
    glEnd();
}

// Sky, Sun, Moon, Stars, Clouds
void drawZone1_Background() {
    // Sky
    if (skyStage == 0) glColor3f(1.0f, 0.6f, 0.3f);
    else if (skyStage == 1) glColor3f(0.3f, 0.7f, 1.0f);
    else if (skyStage == 2) glColor3f(0.1f, 0.4f, 0.9f);
    else glColor3f(0.0f, 0.0f, 0.0f);
    drawRect(0, 120, 1024, 480);

    // Sun / Sunset glow
    bool isNight = (skyStage == 3);
    bool isDay   = (skyStage != 3);

    if (isNight) {
        // Moon
        drawSoftCircle(880, 410, 60, 0.95f,0.92f,0.80f, 0.18f, 0.0f);
        glColor3f(0.98f,0.96f,0.85f);
        drawCircle(880, 410, 28);
        glColor3f(0.05f,0.07f,0.18f);
        drawCircle(870, 415, 24);
        glColor3f(0.85f,0.83f,0.72f);
        drawCircle(885,405,3);
        drawCircle(895,415,2);
        drawCircle(880,420,2.5f);

        // Stars
        float stars[20][3] = {
            { 60,440,1.5f},{130,460,1.2f},{200,430,2.2f},
            {270,455,1.4f},{340,425,1.7f},{410,450,1.0f},
            {480,435,1.9f},{560,460,1.3f},{640,440,1.6f},
            {720,455,1.1f},{780,425,1.8f},{950,460,1.3f},
            {800,380,1.0f},{100,400,1.3f},{380,380,1.5f},
            {620,410,1.0f},{310,470,1.4f},{770,460,1.6f},
            {920,380,1.2f},{510,470,1.0f}
        };
        for (int i = 0; i < 20; i++) {
            float tk = sin(starTwinkle + i * 1.3f) * 0.35f + 0.65f;
            drawSoftCircle(stars[i][0],stars[i][1],stars[i][2]*3.0f,
                           1.0f,1.0f,0.85f, 0.30f*tk, 0.0f);
            glColor3f(1.0f*tk,1.0f*tk,0.95f*tk);
            drawCircle(stars[i][0],stars[i][1],stars[i][2]);
        }
    }

    if (isDay) {
        float gr=1.0f,gg=0.85f,gb=0.40f;
        if (skyStage==2) { gr=1.0f; gg=0.55f; gb=0.15f; }
        drawSoftCircle(150, 430, 90, gr,gg,gb, 0.50f, 0.0f);
        glColor3f(1.0f, skyStage==2 ? 0.70f : 0.95f, skyStage==0 ? 0.40f : 0.25f);
        drawCircle(150, 430, 38);
    }

    // Clouds
    glColor3f(1.0f, 1.0f, 1.0f);
    float cloudBase[4][3] = {
        {120,415,1.0f},
        {400,445,0.8f},
        {680,420,1.1f},
        {900,440,0.9f}
    };

    for (int c = 0; c < 4; c++) {
        float cx = cloudBase[c][0] + cloudShift * (c + 1) * 0.4f;
        if (cx > 1150) cx -= 1300;
        if (cx < -120) cx += 1300;
        float cy  = cloudBase[c][1];
        float scl = cloudBase[c][2];

        if (isNight) {
            glColor3f(0.20f,0.20f,0.28f);
            drawEllipse(cx, cy, 58*scl, 20*scl);
            drawEllipse(cx-45, cy-8, 44*scl, 16*scl);
            drawEllipse(cx+45, cy-5, 46*scl, 17*scl);
            drawEllipse(cx-10, cy+10, 38*scl, 14*scl);
        } else {
            if (skyStage == 2)
                glColor3f(1.0f,0.82f,0.68f);
            else
                glColor3f(0.92f,0.92f,0.95f);
            drawEllipse(cx, cy, 60*scl, 20*scl);
            drawEllipse(cx-50, cy-10, 45*scl, 17*scl);
            drawEllipse(cx+50, cy-5, 48*scl, 18*scl);
            drawEllipse(cx-15, cy+12, 40*scl, 15*scl);
            drawEllipse(cx+20, cy+10, 36*scl, 14*scl);
        }
    }
}

// Ground (simplified)
void drawGround() {
    drawGradRect(0, 0, 1024, 120, 0.42f,0.34f,0.22f, 0.30f,0.26f,0.20f);
    glColor3f(0.25f,0.20f,0.14f);
    float rocks[12][2] = {{50,40},{150,55},{280,25},{380,50},{520,35},{650,55},
                         {780,40},{880,30},{950,45},{100,80},{400,90},{700,75}};
    for (int i=0; i<12; i++) drawCircle(rocks[i][0], rocks[i][1], 2.5f);
}

// USA Jet
void drawUSAJet() {
    glPushMatrix();
    glTranslatef(jetUSAx - 95, 0, 0);
    glColor3f(0.45f,0.48f,0.55f);
    glBegin(GL_POLYGON);
        glVertex2f(50,425); glVertex2f(140,432); glVertex2f(170,428);
        glVertex2f(140,422); glVertex2f(50,418);
    glEnd();
    glColor3f(0.20f,0.30f,0.45f);
    glBegin(GL_POLYGON);
        glVertex2f(125,430); glVertex2f(150,428); glVertex2f(140,425); glVertex2f(125,425);
    glEnd();
    glColor3f(0.40f,0.43f,0.50f);
    glBegin(GL_TRIANGLES);
        glVertex2f(85,430); glVertex2f(115,448); glVertex2f(120,432);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(85,420); glVertex2f(115,402); glVertex2f(120,418);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(60,425); glVertex2f(55,438); glVertex2f(75,428);
    glEnd();

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float jetGlow = 0.6f + sin(t*25)*0.2f;
    drawSoftCircle(50,425,10,1.0f,0.5f,0.2f,jetGlow,0.0f);
    glDisable(GL_BLEND);
    glPopMatrix();
}

// Satellite
void drawSatellite() {
    float y = satBaseY + sin(satPhase) * 20.0f;
    glPushMatrix();
    glTranslatef(satX, y, 0);
    glRotatef(15.0f * sin(satPhase * 0.5f), 0, 0, 1);
    glColor3f(0.78f, 0.78f, 0.82f);
    drawRect(-6, -20, 6, 20);
    glColor3f(0.55f, 0.55f, 0.60f);
    drawRect(-3, -12, 3, 12);
    glColor3f(0.10f, 0.25f, 0.55f);
    drawRect(-45, -10, -6, 10);
    drawRect(6, -10, 45, 10);
    glColor3f(0.05f, 0.15f, 0.35f);
    for (int i = -35; i <= -10; i += 10)

    for (int i = 10; i <= 35; i += 10)

    glPopMatrix();
}

// Intro Explosion
void drawIntroExplosion(float iT) {
    float p = iT / 2.0f;
    if (p > 1.0f) p = 1.0f;

    float cy = 240.0f;
    float cx = 512.0f;

    float leftX  = -150.0f + p * 662.0f;
    float rightX = 1174.0f - p * 662.0f;
    float fRadius = 45.0f + p * 55.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Left Fireball
    drawSoftCircle(leftX, cy, fRadius * 2.2f, 1.0f, 0.30f, 0.00f, 0.30f, 0.0f);
    drawSoftCircle(leftX, cy, fRadius * 1.4f, 1.0f, 0.55f, 0.05f, 0.70f, 0.0f);
    glColor3f(1.0f, 0.75f, 0.20f); drawCircle(leftX, cy, fRadius);
    glColor3f(1.0f, 0.95f, 0.60f); drawCircle(leftX, cy, fRadius * 0.55f);
    glColor3f(1.0f, 1.0f, 0.90f); drawCircle(leftX, cy, fRadius * 0.25f);

    // Right Fireball
    drawSoftCircle(rightX, cy, fRadius * 2.2f, 1.0f, 0.10f, 0.00f, 0.30f, 0.0f);
    drawSoftCircle(rightX, cy, fRadius * 1.4f, 1.0f, 0.35f, 0.00f, 0.70f, 0.0f);
    glColor3f(1.0f, 0.50f, 0.10f); drawCircle(rightX, cy, fRadius);
    glColor3f(1.0f, 0.85f, 0.40f); drawCircle(rightX, cy, fRadius * 0.55f);
    glColor3f(1.0f, 1.0f, 0.90f); drawCircle(rightX, cy, fRadius * 0.25f);

    if (p > 0.82f) {
        float flash = (p - 0.82f) / 0.18f;
        drawSoftCircle(cx, cy, 280.0f * flash, 1.0f, 0.60f, 0.10f, 0.55f * flash, 0.0f);
        drawSoftCircle(cx, cy, 170.0f * flash, 1.0f, 0.75f, 0.20f, 0.85f * flash, 0.0f);
        drawSoftCircle(cx, cy, 90.0f  * flash, 1.0f, 0.90f, 0.60f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(cx, cy, 40.0f * flash);
    }
    glDisable(GL_BLEND);
}

// Intro Title Screen
void drawIntroText() {
    glColor3f(0.0f, 0.0f, 0.0f);
    drawRect(0, 0, 1024, 480);

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(240.0f, 255.0f);
    const char* title = "Iran USA Border Conflict Zone: A 2D Computer Graphics Scene";
    for (const char* c = title; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(450.0f, 225.0f);
    const char* sub = "GROUP- C";
    for (const char* c = sub; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
}

// UPDATE
void update(int value) {
    float dt = 0.03f;

    if (sceneState == INTRO_EXPLOSION) {
        introT += dt;
        if (introT >= 2.0f) {
            sceneState = INTRO_TEXT;
            introT = 0.0f;
        }
        glutPostRedisplay();
        glutTimerFunc(33, update, 0);
        return;
    }

    if (sceneState == INTRO_TEXT) {
        introT += dt;
        if (introT >= 2.0f) {
            sceneState = NORMAL;
        }
        glutPostRedisplay();
        glutTimerFunc(33, update, 0);
        return;
    }

    // NORMAL SCENE
    if (sceneState == NORMAL) {
        t += dt;

        jetUSAx += 5.15f;
        if (jetUSAx > 1100) jetUSAx = -50;

        cloudShift += 0.08f;
        starTwinkle += 0.05f;
        satPhase += 0.03f;

        // Sky cycle (4 stages)
        float skyTime = fmod(t, 8.0f);
        if (skyTime < 2.0f) skyStage = 0;
        else if (skyTime < 4.0f) skyStage = 1;
        else if (skyTime < 6.0f) skyStage = 2;
        else skyStage = 3;
    }

    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
}

// DISPLAY
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (sceneState == INTRO_EXPLOSION) {
        drawIntroExplosion(introT);
    }
    else if (sceneState == INTRO_TEXT) {
        drawIntroText();
    }
    else {
        drawZone1_Background();
        drawSatellite();
        drawGround();
        drawUSAJet();
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0,1024,0,480);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(42);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Iran-USA Border Conflict");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
