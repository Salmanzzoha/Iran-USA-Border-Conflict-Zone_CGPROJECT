//Member 3 ( Suaib Ahmmed Nafees )
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1416f

// VARIABLES
float t           = 0.0f;
float bladeAngle  = 0.0f;
float wheelAngle  = 0.0f;
float treadShift  = 0.0f;
float flagPhase   = 0.0f;
float jetUSAx     = 50.0f;
float windowPulse = 0.0f;
float antennaBlink = 0.0f;
float soldierBob  = 0.0f;

// USA Tank movement
float tankUSAx      = -200.0f;
bool  tankUSADone   = false;
float tankUSATarget = 240.0f;

// =============================================================================
//  HELPER FUNCTIONS
// =============================================================================
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

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
    glEnd();
}

void drawSemiEllipse(float cx, float cy, float rx, float ry) {
    int segments = 30;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float a = PI * i / segments;
        glVertex2f(cx + rx * cos(a), cy + ry * sin(a));
    }
    glEnd();
}

// =============================================================================
//  USA SOLDIER
// =============================================================================
void drawUSASoldierAt(float sx) {
    float sBob = sin(soldierBob) * 0.8f;
    float groundY = 120.0f;
    glColor3f(0.10f, 0.08f, 0.05f);
    drawRect(sx - 5, groundY,
    sx - 1, groundY + 6);
    drawRect(sx + 1, groundY,
    sx + 5, groundY + 6);
    glColor3f(0.30f, 0.36f, 0.20f);
    drawRect(sx - 5, groundY + 6,
    sx - 1, groundY + 18 + sBob);
    drawRect(sx + 1, groundY + 6,
    sx + 5, groundY + 18 + sBob);
    glColor3f(0.32f, 0.40f, 0.22f);
    drawRect(sx - 6, groundY + 16 +
    sBob, sx + 6, groundY + 34 + sBob);
    glColor3f(0.20f, 0.26f, 0.14f);
    drawRect(sx - 5, groundY + 22 + sBob,
    sx + 5, groundY + 30 + sBob);
    float sy = groundY + 38.0f + sBob;
    glColor3f(0.92f, 0.76f, 0.60f);
    drawCircle(sx, sy, 4);
    glColor3f(0.25f, 0.32f, 0.18f);
    drawSemiEllipse(sx, sy + 1, 5, 4);
    glColor3f(0.18f, 0.24f, 0.14f);
    drawLine(sx - 5, sy + 1, sx + 5, sy + 1);
    glColor3f(0.30f, 0.38f, 0.20f);
    glLineWidth(2.5f);
    drawLine(sx - 5, groundY + 30 + sBob,
    sx - 9, groundY + 18 + sBob);
    drawLine(sx + 5, groundY + 30 + sBob,
    sx + 14, groundY + 26 + sBob);
    glLineWidth(1.0f);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawRect(sx + 14, groundY + 23 + sBob,
    sx + 32, groundY + 27 + sBob);
    drawRect(sx + 11, groundY + 22 + sBob,
    sx + 16, groundY + 28 + sBob);
    drawRect(sx + 19, groundY + 27 + sBob,
    sx + 23, groundY + 30 + sBob);
}

// =============================================================================
//  Helper: Waving flag strip
// =============================================================================
void drawWavingStripe(float x1, float yBot, float yTop, float w,
                      float r, float g, float b, float phase) {
    int seg=12;
    glColor3f(r,g,b);
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<=seg;i++) {
        float u=(float)i/seg;
        float x=x1+u*w;
        float wave=sin(u*4+phase)*3.0f*u;
        glVertex2f(x,yBot+wave); glVertex2f(x,yTop+wave);
    }
    glEnd();
}

// =============================================================================
//  Helper: Rolling tank tread
// =============================================================================
void drawTankWheels(float wx[3], float wy, float r, float wheelAng) {
    for (int i=0;i<3;i++) {
        glColor3f(0.08f,0.08f,0.08f);
        drawCircle(wx[i],wy,r);
        glPushMatrix();
        glTranslatef(wx[i],wy,0);
        glRotatef(wheelAng,0,0,1);
        glColor3f(0.30f,0.30f,0.30f);
        drawCircle(0,0,5);
        glColor3f(0.10f,0.10f,0.10f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
            glVertex2f(-5,0);
            glVertex2f(5,0);
            glVertex2f(0,-5);
            glVertex2f(0,5);
        glEnd();
        glLineWidth(1.0f);
        glPopMatrix();
    }
}

void drawScrollingTread(float x1, float x2, float y1, float y2, float shift) {
    glColor3f(0.05f,0.05f,0.05f); drawRect(x1,y1,x2,y2);
    glColor3f(0.12f,0.12f,0.12f);
    float spacing=8.0f, offset=fmod(shift,spacing);
    for (float x=x1+offset;x<x2;x+=spacing) {
        float xa=x, xb=x+5;
        if (xa<x1) xa=x1; if (xb>x2) xb=x2;
        if (xb>xa) drawRect(xa,y1,xb,y1+4);
    }
}

// =============================================================================
//  GROUND (simple ground so USA elements have something to stand on)
// =============================================================================
void drawGround() {
    // sky background
    glColor3f(0.75f, 0.90f, 1.00f);
    drawRect(0, 120, 1024, 480);

    // ground
    drawGradRect(0,0,1024,120, 0.42f,0.34f,0.22f, 0.30f,0.26f,0.20f);
    glColor3f(0.25f,0.20f,0.14f);
    float rocks[12][2]={{50,40},{120,30},{200,55},{280,25},{350,50},{420,35},
                        {90,80},{260,75},{390,90},{600,85},{720,75},{890,85}};
    for (int i=0;i<12;i++) drawCircle(rocks[i][0],rocks[i][1],2.5f);
}

// =============================================================================
//  MEMBER 3 — USA Side
// =============================================================================
void drawZone3_USA() {
    glColor3f(0.20f,0.25f,0.14f);
    drawRect(98,120,122,200);
    glColor3f(0.15f,0.20f,0.10f);
    drawLine(98,120,122,200);
    drawLine(122,120,98,200);
    drawGradRect(80,200,140,320, 0.22f,0.30f,0.16f, 0.30f,0.40f,0.20f);
    glColor3f(0.16f,0.22f,0.10f);
    drawLine(95,200,95,320);
    drawLine(110,200,110,320);
    drawLine(125,200,125,320);
    glColor3f(0.15f,0.20f,0.10f);
    drawRect(68,318,152,322);
    drawGradRect(70,322,150,350, 0.20f,0.28f,0.13f, 0.28f,0.36f,0.18f);

    glColor3f(0.10f,0.10f,0.10f);
    glLineWidth(1.5f);
    drawLine(110,350,110,365);
    glLineWidth(1.0f);
    float blink=(sin(antennaBlink)>0)?1.0f:0.2f;
    drawSoftCircle(110,366,5,1.0f,0.1f,0.1f,0.8f*blink,0.0f);
    glColor3f(blink,0.1f*blink,0.1f*blink); drawCircle(110,366,1.8f);

    float wp=0.3f+sin(windowPulse)*0.05f;
    drawSoftCircle(95,270,22,1.0f,0.85f,0.5f,wp,0.0f);
    drawSoftCircle(125,270,22,1.0f,0.85f,0.5f,wp,0.0f);
    glColor3f(0.85f,0.78f,0.55f);
    drawRect(88,260,102,280);
    drawRect(118,260,132,280);
    glColor3f(0.10f,0.12f,0.08f);
    drawLine(95,260,95,280);
    drawLine(125,260,125,280);
    drawLine(88,270,102,270);
    drawLine(118,270,132,270);

    float poleX = 75.0f;
    glColor3f(0.85f,0.85f,0.85f);
    glLineWidth(2.0f);
    drawLine(poleX, 322, poleX, 418);
    glLineWidth(1.0f);

    float fx=poleX, fy=380, fw=50;
    for (int i=0;i<7;i++) {
        float r=(i%2==0)?0.78f:0.95f;
        float g=(i%2==0)?0.10f:0.95f;
        float b=(i%2==0)?0.10f:0.95f;
        drawWavingStripe(fx,fy+i*5,fy+(i+1)*5,fw,r,g,b,flagPhase);
    }
    glColor3f(0.10f,0.18f,0.50f);
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<=8;i++) {
        float u=(float)i/8;
        float x=fx+u*22;
        float wave=sin(u*4+flagPhase)*3.0f*u;
        glVertex2f(x,400+wave); glVertex2f(x,415+wave);
    }
    glEnd();
    glColor3f(1,1,1);
    for (int i=0;i<3;i++)
        for (int j=0;j<4;j++) {
            float u=((float)i*7+3)/22;
            float wave=sin(u*4+flagPhase)*3.0f*u;
            drawCircle(fx+3.0f+i*7, 402.0f+j*4+wave, 0.6f);
        }

    float tx = tankUSAx;
    drawScrollingTread(tx,      tx+124, 96, 124, treadShift);
    drawGradRect(tx-8,  122, tx+132, 145, 0.16f,0.24f,0.12f, 0.26f,0.36f,0.18f);
    glColor3f(0.10f,0.16f,0.08f);
    drawRect(tx+27, 142, tx+97, 145);
    glColor3f(0.20f,0.30f,0.14f);
    drawRect(tx+27, 145, tx+97, 168);
    drawEllipse(tx+62, 168, 36, 6);
    glColor3f(0.10f,0.14f,0.08f); drawCircle(tx+62, 165, 5);
    glColor3f(0.20f,0.28f,0.14f); drawCircle(tx+62, 166, 3);
    glColor3f(0.12f,0.16f,0.08f);
    drawRect(tx+95,  152, tx+192, 162);
    glColor3f(0.05f,0.05f,0.05f);
    drawRect(tx+190, 151, tx+197, 163);
    drawCircle(tx+197, 157, 2);
    float wx[3]={tx+17, tx+62, tx+107};
    drawTankWheels(wx, 110, 14, wheelAngle);
    glColor3f(0.30f,0.40f,0.20f);
    drawRect(tx-3,  130, tx+7,  145);
    glColor3f(0.15f,0.20f,0.10f);
    drawRect(tx,    144, tx+4,  147);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f,0.45f,0.15f,0.18f);
    glDisable(GL_BLEND);

    float sBob=sin(soldierBob)*0.8f;
    float groundY=120.0f;
    float sx=395;
    glColor3f(0.10f,0.08f,0.05f);
    drawRect(sx-5,groundY,    sx-1,groundY+6);
    drawRect(sx+1,groundY,    sx+5,groundY+6);
    glColor3f(0.30f,0.36f,0.20f);
    drawRect(sx-5,groundY+6,  sx-1,groundY+18+sBob);
    drawRect(sx+1,groundY+6,  sx+5,groundY+18+sBob);
    glColor3f(0.32f,0.40f,0.22f);
    drawRect(sx-6,groundY+16+sBob,sx+6,groundY+34+sBob);
    glColor3f(0.20f,0.26f,0.14f);
    drawRect(sx-5,groundY+22+sBob,sx+5,groundY+30+sBob);
    glColor3f(0.92f,0.76f,0.60f);
    float sy=groundY+38+sBob;
    drawCircle(sx,sy,4);
    glColor3f(0.25f,0.32f,0.18f); drawSemiEllipse(sx,sy+1,5,4);
    glColor3f(0.18f,0.24f,0.14f); drawLine(sx-5,sy+1,sx+5,sy+1);
    glColor3f(0.30f,0.38f,0.20f); glLineWidth(2.5f);
    drawLine(sx-5,groundY+30+sBob,sx-9,groundY+18+sBob);
    drawLine(sx+5,groundY+30+sBob,sx+14,groundY+26+sBob);
    glLineWidth(1.0f);
    glColor3f(0.10f,0.10f,0.10f);
    drawRect(sx+14,groundY+23+sBob,sx+32,groundY+27+sBob);
    drawRect(sx+11,groundY+22+sBob,sx+16,groundY+28+sBob);
    drawRect(sx+19,groundY+27+sBob,sx+23,groundY+30+sBob);

    drawUSASoldierAt(157.0f);
    drawUSASoldierAt(172.0f);
    drawUSASoldierAt(187.0f);

    glPushMatrix();
    glTranslatef(jetUSAx-95,0,0);
    glColor3f(0.45f,0.48f,0.55f);
    glBegin(GL_POLYGON);
        glVertex2f(50,425);glVertex2f(140,432);glVertex2f(170,428);
        glVertex2f(140,422);glVertex2f(50,418);
    glEnd();
    glColor3f(0.20f,0.30f,0.45f);
    glBegin(GL_POLYGON);
        glVertex2f(125,430);glVertex2f(150,428);glVertex2f(140,425);glVertex2f(125,425);
    glEnd();
    glColor3f(0.40f,0.43f,0.50f);
    glBegin(GL_TRIANGLES);
        glVertex2f(85,430);glVertex2f(115,448);glVertex2f(120,432);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(85,420);glVertex2f(115,402);glVertex2f(120,418);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(60,425);glVertex2f(55,438);glVertex2f(75,428);
    glEnd();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    float jetGlow=0.6f+sin(t*25)*0.2f;
    drawSoftCircle(50,425,10,1.0f,0.5f,0.2f,jetGlow,0.0f);
    glDisable(GL_BLEND);
    glPopMatrix();
}

// =============================================================================
//  DISPLAY
// =============================================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawGround();
    drawZone3_USA();

    glutSwapBuffers();
}

// =============================================================================
//  ANIMATION TIMER
// =============================================================================
void update(int value) {
    t += 0.03f;

    wheelAngle += 6.0f;
    if (wheelAngle > 360) wheelAngle -= 360;
    treadShift -= 1.0f;

    flagPhase    += 0.20f;
    windowPulse  += 0.04f;
    antennaBlink += 0.1f;
    soldierBob   += 0.08f;

    jetUSAx += 5.15f;
    if (jetUSAx > 1100) jetUSAx = -50;

    if (!tankUSADone) {
        tankUSAx += 2.0f;
        if (tankUSAx >= 228.0f) { tankUSAx = 228.0f; tankUSADone = true; }
    }

    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
}

void init() {
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0,1024,0,480);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    srand(42);
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1024,480);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Member 3 - USA Side");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0,update,0);
    glutMainLoop();
    return 0;
}
