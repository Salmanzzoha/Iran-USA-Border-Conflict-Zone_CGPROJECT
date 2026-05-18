//Member 4 ( Samiha Sultana )
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1416f

// VARIABLES
float t            = 0.0f;
float heliX        = -120.0f;
float heliY        = 380.0f;
float bladeAngle   = 0.0f;
float wheelAngle   = 0.0f;
float treadShift   = 0.0f;
float flagPhase    = 0.0f;
float jetIRNx      = 974.0f;
float windowPulse  = 0.0f;
float antennaBlink = 0.0f;
float soldierBob   = 0.0f;

// Iran Tank movement
float tankIRNx      = 1300.0f;
bool  tankIRNDone   = false;
float tankIRNTarget = 740.0f;

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
//  IRAN SOLDIER
// =============================================================================
void drawIRNSoldierAt(float sx) {
    float sBob = sin(soldierBob + PI) * 0.8f;
    float groundY = 120.0f;
    glColor3f(0.10f, 0.08f, 0.05f);
    drawRect(sx - 5, groundY,
    sx - 1, groundY + 6);
    drawRect(sx + 1, groundY,
    sx + 5, groundY + 6);
    glColor3f(0.42f, 0.30f, 0.18f);
    drawRect(sx - 5, groundY + 6,
    sx - 1, groundY + 18 + sBob);
    drawRect(sx + 1, groundY + 6,
    sx + 5, groundY + 18 + sBob);
    glColor3f(0.46f, 0.34f, 0.22f);
    drawRect(sx - 6, groundY + 16 + sBob,
    sx + 6, groundY + 34 + sBob);
    glColor3f(0.30f, 0.22f, 0.14f);
    drawRect(sx - 5, groundY + 22 + sBob,
    sx + 5, groundY + 30 + sBob);
    float sy = groundY + 38.0f + sBob;
    glColor3f(0.92f, 0.76f, 0.60f);
    drawCircle(sx, sy, 4);
    glColor3f(0.36f, 0.26f, 0.16f);
    drawSemiEllipse(sx, sy + 1, 5, 4);
    glColor3f(0.24f, 0.18f, 0.12f);
    drawLine(sx - 5, sy + 1, sx + 5, sy + 1);
    glColor3f(0.42f, 0.30f, 0.18f);
    glLineWidth(2.5f);
    drawLine(sx + 5, groundY + 30 + sBob,
    sx + 9, groundY + 18 + sBob);
    drawLine(sx - 5, groundY + 30 + sBob,
    sx - 14, groundY + 26 + sBob);
    glLineWidth(1.0f);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawRect(sx - 32, groundY + 23 + sBob,
    sx - 14, groundY + 27 + sBob);
    drawRect(sx - 16, groundY + 22 + sBob,
    sx - 11, groundY + 28 + sBob);
    drawRect(sx - 23, groundY + 27 + sBob,
    sx - 19, groundY + 30 + sBob);
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
//  GROUND (simple ground so Iran elements have something to stand on)
// =============================================================================
void drawGround() {
    // sky background
    glColor3f(0.75f, 0.90f, 1.00f);
    drawRect(0, 120, 1024, 480);

    // ground
    drawGradRect(0,0,1024,120, 0.46f,0.36f,0.24f, 0.32f,0.28f,0.22f);
    glColor3f(0.25f,0.20f,0.14f);
    float rocks[12][2]={{50,40},{120,30},{200,55},{280,25},{350,50},{420,35},
                        {580,40},{640,55},{710,30},{780,50},{860,30},{940,45}};
    for (int i=0;i<12;i++) drawCircle(rocks[i][0],rocks[i][1],2.5f);
}

// =============================================================================
//  MEMBER 4 — Iran Side
// =============================================================================
void drawZone4_Iran() {
    glColor3f(0.32f,0.25f,0.15f);
    drawRect(902,120,926,200);
    glColor3f(0.22f,0.16f,0.10f);
    drawLine(902,120,926,200);
    drawLine(926,120,902,200);
    drawGradRect(884,200,944,320, 0.40f,0.30f,0.18f, 0.55f,0.42f,0.25f);
    glColor3f(0.32f,0.24f,0.14f);
    drawLine(899,200,899,320);
    drawLine(914,200,914,320);
    drawLine(929,200,929,320);
    glColor3f(0.30f,0.22f,0.12f);
    drawRect(872,318,956,322);
    drawGradRect(874,322,954,350, 0.36f,0.27f,0.15f, 0.46f,0.34f,0.20f);

    glColor3f(0.10f,0.10f,0.10f);
    glLineWidth(1.5f);
    drawLine(914,350,914,365);
    glLineWidth(1.0f);
    float blink=(sin(antennaBlink+PI)>0)?1.0f:0.2f;
    drawSoftCircle(914,366,5,0.1f,1.0f,0.3f,0.8f*blink,0.0f);
    glColor3f(0.1f*blink,blink,0.3f*blink); drawCircle(914,366,1.8f);

    float wp=0.3f+sin(windowPulse+PI/2)*0.05f;
    drawSoftCircle(899,270,22,1.0f,0.85f,0.5f,wp,0.0f);
    drawSoftCircle(929,270,22,1.0f,0.85f,0.5f,wp,0.0f);
    glColor3f(0.85f,0.78f,0.55f);
    drawRect(892,260,906,280);
    drawRect(922,260,936,280);
    glColor3f(0.10f,0.08f,0.05f);
    drawLine(899,260,899,280);
    drawLine(929,260,929,280);
    drawLine(892,270,906,270);
    drawLine(922,270,936,270);

    float poleX = 960.0f;
    glColor3f(0.85f,0.85f,0.85f);
    glLineWidth(2.0f);
    drawLine(poleX, 322, poleX, 418);
    glLineWidth(1.0f);

    int seg=12;
    float fxR=poleX, fxL=poleX-50;
    glColor3f(0.0f,0.55f,0.30f);
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<=seg;i++) {
        float u=(float)i/seg;
        float x=fxL+u*(fxR-fxL);
        float wave=sin((1-u)*4+flagPhase)*3.0f*(1-u);
        glVertex2f(x,405+wave); glVertex2f(x,418+wave);
    }
    glEnd();
    glColor3f(0.95f,0.95f,0.95f);
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<=seg;i++) {
        float u=(float)i/seg;
        float x=fxL+u*(fxR-fxL);
        float wave=sin((1-u)*4+flagPhase)*3.0f*(1-u);
        glVertex2f(x,392+wave); glVertex2f(x,405+wave);
    }
    glEnd();
    glColor3f(0.82f,0.10f,0.10f);
    glBegin(GL_QUAD_STRIP);
    for (int i=0;i<=seg;i++) {
        float u=(float)i/seg;
        float x=fxL+u*(fxR-fxL);
        float wave=sin((1-u)*4+flagPhase)*3.0f*(1-u);
        glVertex2f(x,379+wave); glVertex2f(x,392+wave);
    }
    glEnd();
    {
        float u=(fxL+25-fxL)/(fxR-fxL);
        float wave=sin((1-u)*4+flagPhase)*3.0f*(1-u);
        glColor3f(0.82f,0.10f,0.10f);
        drawCircle(fxL+25,398+wave,4);
        glColor3f(0.95f,0.95f,0.95f);
        drawCircle(fxL+25,398+wave,1.5f);
        drawLine(fxL+25,394+wave,fxL+25,402+wave);
        drawLine(fxL+21,398+wave,fxL+29,398+wave);
    }

    float tx = tankIRNx - 132;
    drawScrollingTread(tx,tx+124, 96, 124, treadShift);
    drawGradRect(tx-8,  122, tx+140, 145, 0.50f,0.42f,0.26f, 0.65f,0.55f,0.35f);
    glColor3f(0.40f,0.32f,0.20f);
    drawRect(tx+27, 142, tx+97, 145);
    glColor3f(0.62f,0.52f,0.32f);
    drawRect(tx+27, 145, tx+97, 168);
    drawEllipse(tx+70, 168, 36, 6);
    glColor3f(0.30f,0.24f,0.14f);
    drawCircle(tx+70, 165, 5);
    glColor3f(0.55f,0.45f,0.28f);
    drawCircle(tx+70, 166, 3);
    glColor3f(0.40f,0.32f,0.20f);
    drawRect(tx-60, 152, tx+27,  162);
    glColor3f(0.05f,0.05f,0.05f);
    drawRect(tx-67, 151, tx-60,  163);
    drawCircle(tx-67, 157, 2);
    float wx[3]={tx+25, tx+70, tx+115};
    drawTankWheels(wx, 110, 14, wheelAngle);
    glColor3f(0.55f,0.42f,0.25f);
    drawRect(tx+125, 130, tx+135, 145);
    glColor3f(0.30f,0.22f,0.12f);
    drawRect(tx+128, 144, tx+132, 147);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f,0.45f,0.15f,0.20f);
    glDisable(GL_BLEND);

    float sBob=sin(soldierBob+PI)*0.8f;
    float groundY=120.0f;
    float sx=630;
    glColor3f(0.10f,0.08f,0.05f);
    drawRect(sx-5,groundY,sx-1,groundY+6);
    drawRect(sx+1,groundY,sx+5,groundY+6);
    glColor3f(0.42f,0.30f,0.18f);
    drawRect(sx-5,groundY+6,  sx-1,groundY+18+sBob);
    drawRect(sx+1,groundY+6,  sx+5,groundY+18+sBob);
    glColor3f(0.46f,0.34f,0.22f);
    drawRect(sx-6,groundY+16+sBob,sx+6,groundY+34+sBob);
    glColor3f(0.30f,0.22f,0.14f);
    drawRect(sx-5,groundY+22+sBob,sx+5,groundY+30+sBob);
    glColor3f(0.92f,0.76f,0.60f);
    float sy2=groundY+38+sBob;
    drawCircle(sx,sy2,4);
    glColor3f(0.36f,0.26f,0.16f); drawSemiEllipse(sx,sy2+1,5,4);
    glColor3f(0.24f,0.18f,0.12f); drawLine(sx-5,sy2+1,sx+5,sy2+1);
    glColor3f(0.42f,0.30f,0.18f); glLineWidth(2.5f);
    drawLine(sx+5,groundY+30+sBob,sx+9,groundY+18+sBob);
    drawLine(sx-5,groundY+30+sBob,sx-14,groundY+26+sBob);
    glLineWidth(1.0f);
    glColor3f(0.10f,0.10f,0.10f);
    drawRect(sx-32,groundY+23+sBob,sx-14,groundY+27+sBob);
    drawRect(sx-16,groundY+22+sBob,sx-11,groundY+28+sBob);
    drawRect(sx-23,groundY+27+sBob,sx-19,groundY+30+sBob);

    drawIRNSoldierAt(860.0f);
    drawIRNSoldierAt(845.0f);
    drawIRNSoldierAt(830.0f);

    glPushMatrix();
    glTranslatef(jetIRNx-974,0,0);
    glColor3f(0.50f,0.42f,0.28f);
    glBegin(GL_POLYGON);
        glVertex2f(974,425);glVertex2f(884,432);glVertex2f(854,428);
        glVertex2f(884,422);glVertex2f(974,418);
    glEnd();
    glColor3f(0.30f,0.20f,0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(899,430);glVertex2f(874,428);glVertex2f(884,425);glVertex2f(899,425);
    glEnd();
    glColor3f(0.45f,0.38f,0.25f);
    glBegin(GL_TRIANGLES);
        glVertex2f(939,430);glVertex2f(909,448);glVertex2f(904,432);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(939,420);glVertex2f(909,402);glVertex2f(904,418);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(964,425);glVertex2f(969,438);glVertex2f(949,428);
    glEnd();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    float jetGlow=0.6f+sin(t*25+PI)*0.2f;
    drawSoftCircle(974,425,10,1.0f,0.5f,0.2f,jetGlow,0.0f);
    glDisable(GL_BLEND);
    glPopMatrix();
}

// =============================================================================
//  HELICOPTER
// =============================================================================
void drawHelicopter() {
    glPushMatrix();
    glTranslatef(heliX,heliY,0);
    glRotatef(4,0,0,1);
    float beamSwing=cos(t*1.5f)*12.0f;
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUAD_STRIP);
        glColor4f(1.0f,0.95f,0.7f,0.45f); glVertex2f(20,-6); glVertex2f(35,-6);
        glColor4f(1.0f,0.95f,0.7f,0.0f);
        glVertex2f(-30+beamSwing,-90); glVertex2f(80+beamSwing,-90);
    glEnd();
    glDisable(GL_BLEND);
    glBegin(GL_POLYGON);
        glColor3f(0.18f,0.22f,0.22f); glVertex2f(-40,-10);
        glColor3f(0.22f,0.26f,0.26f); glVertex2f(20,-10);
        glColor3f(0.26f,0.30f,0.30f); glVertex2f(40,0);
        glColor3f(0.30f,0.34f,0.34f); glVertex2f(20,12);
        glColor3f(0.20f,0.24f,0.24f); glVertex2f(-40,10);
    glEnd();
    glColor3f(0.10f,0.12f,0.12f); drawRect(-40,-12,20,-9);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.20f,0.40f,0.55f,0.85f);
    glBegin(GL_POLYGON);
        glVertex2f(10,-5); glVertex2f(30,0); glVertex2f(10,8);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.45f,0.55f,0.60f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(10,-5); glVertex2f(30,0); glVertex2f(10,8);
    glEnd();
    glColor3f(0.10f,0.18f,0.25f); drawRect(-5,-2,10,6);
    glColor3f(0.10f,0.12f,0.12f); drawLine(-15,-10,-15,10);
    glColor3f(0.20f,0.24f,0.24f); drawRect(-90,-3,-40,3);
    glColor3f(0.14f,0.18f,0.18f); drawRect(-90,-3,-40,-1);
    glColor3f(0.22f,0.26f,0.26f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-90,3); glVertex2f(-100,18); glVertex2f(-78,8);
    glEnd();
    glColor3f(0.18f,0.22f,0.22f); drawRect(-95,-2,-82,1);
    glPushMatrix();
    glTranslatef(-95,8,0); glRotatef(bladeAngle*3,0,0,1);
    glColor3f(0.05f,0.05f,0.05f); glLineWidth(1.5f);
    glBegin(GL_LINES);
        glVertex2f(-7,0); glVertex2f(7,0); glVertex2f(0,-7); glVertex2f(0,7);
    glEnd();
    glLineWidth(1.0f);
    drawSoftCircle(0,0,8,0.3f,0.3f,0.3f,0.25f,0.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,14,0); glRotatef(bladeAngle,0,0,1);
    glColor3f(0.05f,0.05f,0.05f);
    for (int i=0;i<4;i++) {
        glRotatef(90,0,0,1);
        glBegin(GL_QUADS);
            glVertex2f(0,-1.2f); glVertex2f(60,-1.2f);
            glVertex2f(60,1.2f); glVertex2f(0,1.2f);
        glEnd();
    }
    glPopMatrix();
    drawSoftCircle(0,14,60,0.4f,0.4f,0.4f,0.10f,0.0f);
    glColor3f(0.10f,0.10f,0.10f); drawRect(-2,12,2,16);
    glColor3f(0.30f,0.30f,0.30f); drawCircle(0,14,3);
    float pulse=(sin(t*6)>0)?1.0f:0.4f;
    drawSoftCircle(-40,0,7,1.0f,0.0f,0.0f,0.9f*pulse,0.0f);
    drawSoftCircle(40,0,7,0.0f,1.0f,0.0f,0.9f*pulse,0.0f);
    drawSoftCircle(-100,18,6,1.0f,1.0f,1.0f,0.8f*pulse,0.0f);
    glColor3f(1.0f*pulse,0.2f*pulse,0.2f*pulse); drawCircle(-40,0,1.5f);
    glColor3f(0.2f*pulse,1.0f*pulse,0.2f*pulse); drawCircle(40,0,1.5f);
    glColor3f(pulse,pulse,pulse);                 drawCircle(-100,18,1.2f);
    glColor3f(0.08f,0.08f,0.08f); glLineWidth(2.0f);
    drawLine(-25,-10,-32,-22); drawLine(-32,-22,28,-22);
    drawLine(28,-22,32,-10);   drawLine(-12,-10,-12,-22);
    drawLine(12,-10,12,-22);   glLineWidth(1.0f);
    glPopMatrix();
}

// =============================================================================
//  DISPLAY
// =============================================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawGround();
    drawZone4_Iran();
    drawHelicopter();

    glutSwapBuffers();
}

// =============================================================================
//  ANIMATION TIMER
// =============================================================================
void update(int value) {
    t += 0.03f;

    heliX += 4.5f;
    if (heliX > 1150) heliX = -120;
    heliY = 380 + sin(t * 1.5f) * 4;
    bladeAngle += 18.0f;
    if (bladeAngle > 360) bladeAngle -= 360;

    wheelAngle += 6.0f;
    if (wheelAngle > 360) wheelAngle -= 360;
    treadShift -= 1.0f;

    flagPhase    += 0.20f;
    windowPulse  += 0.04f;
    antennaBlink += 0.1f;
    soldierBob   += 0.08f;

    jetIRNx -= 5.15f;
    if (jetIRNx < -50) jetIRNx = 1100;

    if (!tankIRNDone) {
        tankIRNx -= 2.0f;
        if (tankIRNx <= 806.0f) { tankIRNx = 806.0f; tankIRNDone = true; }
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
    glutCreateWindow("Member 4 - Iran Side");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0,update,0);
    glutMainLoop();
    return 0;
}
