//Member 2 ( Salman Zzoha  22-49602-3)
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1416f

// VARIABLES
float t = 0.0f;
float jetIRNx = 974.0f;
float skyStage = 1;

// Explosion
float shockwave = 0.0f;
float missileT = 0.0f;

// Drones
float droneUSAx[3] = {80.0f, 370.0f, 660.0f};
float droneIRNx[3] = {944.0f, 654.0f, 364.0f};

// Smoke
#define MAX_SMOKE 60
struct SmokeP { float x, y, vx, vy, age, life, size; bool alive; };
SmokeP smoke[MAX_SMOKE];


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

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1,y1); glVertex2f(x2,y2);
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

void drawRing(float cx, float cy, float rIn, float rOut,
              float r, float g, float b, float a) {
    int seg = 64;
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, a);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= seg; i++) {
        float ang = 2.0f * PI * i / seg;
        glVertex2f(cx + rIn  * cos(ang), cy + rIn  * sin(ang));
        glVertex2f(cx + rOut * cos(ang), cy + rOut * sin(ang));
    }
    glEnd();
    glDisable(GL_BLEND);
}

float frand(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

// SMOKE
void spawnSmoke() {
    for (int i = 0; i < MAX_SMOKE; i++) {
        if (!smoke[i].alive) {
            smoke[i].x = 512 + frand(-12, 12);
            smoke[i].y = 215 + frand(-5, 5);
            smoke[i].vx = frand(-0.2f, 0.7f);
            smoke[i].vy = frand(0.5f, 1.3f);
            smoke[i].age = 0; smoke[i].life = frand(2.8f, 5.0f);
            smoke[i].size = frand(16, 26);
            smoke[i].alive = true;
            return;
        }
    }
}

void drawSmoke() {
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < MAX_SMOKE; i++) {
        if (!smoke[i].alive) continue;
        float a = smoke[i].age / smoke[i].life;
        float alpha = (1.0f - a) * 0.65f;
        float gray = 0.22f + a * 0.35f;
        float size = smoke[i].size * (1.0f + a * 1.3f);

        glColor4f(gray, gray, gray + 0.03f, alpha * 0.6f);
        drawEllipse(smoke[i].x, smoke[i].y, size * 1.45f, size * 0.9f);

        glColor4f(gray, gray, gray + 0.02f, alpha);
        drawEllipse(smoke[i].x, smoke[i].y, size, size * 0.65f);
    }
    glDisable(GL_BLEND);
}

// DRONES
void drawDrone(float x, float y, bool isUSA) {
    if (isUSA) glColor3f(0.20f, 0.28f, 0.16f);
    else       glColor3f(0.48f, 0.36f, 0.20f);
    drawRect(x - 8, y - 3, x + 8, y + 3);

    glColor3f(0.12f, 0.12f, 0.12f);
    glLineWidth(1.5f);
    drawRect(x - 18, y - 1, x - 8, y + 1);
    drawRect(x + 8, y - 1, x + 18, y + 1);
    glLineWidth(1.0f);

    if (isUSA) glColor3f(0.10f, 0.18f, 0.10f);
    else       glColor3f(0.30f, 0.22f, 0.12f);
    drawCircle(x - 18, y, 4.0f);
    drawCircle(x + 18, y, 4.0f);
}

void drawDrones() {
    float baseY = 355.0f;
    for (int i = 0; i < 3; i++) {
        float wave = sin(t * 2.5f + i * 0.9f) * 6.0f;
        drawDrone(droneUSAx[i], baseY + wave, true);
        drawDrone(droneIRNx[i], baseY + wave, false);
    }
}



//  SKY
void drawSky() {
    if (skyStage == 0) glColor3f(1.0f, 0.6f, 0.3f);
    else if (skyStage == 1) glColor3f(0.3f, 0.7f, 1.0f);
    else if (skyStage == 2) glColor3f(0.1f, 0.4f, 0.9f);
    else glColor3f(0.0f, 0.0f, 0.0f);

    drawRect(0, 120, 1024, 480);

    if (skyStage == 3) {
        drawSoftCircle(880, 410, 60, 0.95f,0.92f,0.80f, 0.18f, 0.0f);
        glColor3f(0.98f,0.96f,0.85f); drawCircle(880, 410, 28);
    } else {
        float gr=1.0f, gg=0.85f, gb=0.40f;
        if (skyStage==2) { gr=1.0f; gg=0.55f; gb=0.15f; }
        drawSoftCircle(150, 430, 90, gr,gg,gb, 0.50f, 0.0f);
        glColor3f(1.0f, skyStage==2 ? 0.70f : 0.95f, skyStage==0 ? 0.40f : 0.25f);
        drawCircle(150, 430, 38);
    }
}

// BORDER
void drawZone2_Border() {
    drawGradRect(0,0,500,120, 0.42f,0.34f,0.22f, 0.30f,0.26f,0.20f);
    drawGradRect(524,0,1024,120, 0.46f,0.36f,0.24f, 0.32f,0.28f,0.22f);
    glColor3f(0.25f,0.20f,0.14f);
    float rocks[18][2]={{50,40},{120,30},{200,55},{280,25},{350,50},{420,35},
                        {580,40},{640,55},{710,30},{780,50},{860,30},{940,45},
                        {90,80},{260,75},{390,90},{600,85},{720,75},{890,85}};
    for (int i=0;i<18;i++) drawCircle(rocks[i][0],rocks[i][1],2.5f);
    glColor3f(0.55f,0.46f,0.30f);
    for (int i=0;i<10;i++) { float x=30+i*95; drawLine(x,30,x+25,35); }
    drawGradRect(500,0,524,280, 0.18f,0.18f,0.20f, 0.30f,0.30f,0.32f);
    glColor3f(0.12f,0.12f,0.14f);
    for (int y=30;y<280;y+=30) drawLine(500,y,524,y);
    glColor3f(0.55f,0.55f,0.58f); glLineWidth(1.2f);
    for (int row=0;row<3;row++) {
        float yTop=200+row*20;
        glBegin(GL_LINE_STRIP);
            glVertex2f(485,yTop);
             glVertex2f(512,yTop-2);
             glVertex2f(539,yTop);
        glEnd();
    }
    glLineWidth(1.0f);
    drawGradRect(485,0,500,290, 0.25f,0.25f,0.27f, 0.18f,0.18f,0.20f);
    drawGradRect(524,0,539,290, 0.25f,0.25f,0.27f, 0.18f,0.18f,0.20f);
    glColor3f(0.55f,0.55f,0.58f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_STRIP);
    for (int x=20;x<=480;x+=20) {
        float y=105.0f+(((x/20)%2==0)?6.0f:0.0f);
        glVertex2f((float)x,y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (int x=544;x<=1004;x+=20) {
        float y=105.0f+(((x/20)%2==0)?6.0f:0.0f);
        glVertex2f((float)x,y);
    }
    glEnd();
    for (int x=30;x<=470;x+=40) {
        drawLine(x,108,x-2,113); drawLine(x,108,x+2,113);
    }
    for (int x=554;x<=994;x+=40) {
        drawLine(x,108,x-2,113); drawLine(x,108,x+2,113);
    }
    glLineWidth(1.0f);
    for (int side=0;side<2;side++) {
        float baseX=(side==0)?440:584;
        for (int row=0;row<3;row++) {
            float y=95.0f+row*13.0f;
            float x=baseX+(row%2==0?0:(side==0?8:-8));
            glColor3f(0.68f+side*0.05f,0.58f,0.38f);
            drawEllipse(x,y,18,7);
            glColor3f(0.80f+side*0.03f,0.70f,0.48f);
            drawEllipse(x,y+2,14,3);
            glColor3f(0.40f,0.32f,0.20f);
            drawCircle(x,y+3,1.2f);
        }
    }
    glColor3f(0.10f,0.08f,0.05f);
    drawSemiEllipse(180,100,25,30);
    glColor3f(0.32f,0.27f,0.20f);
    drawSemiEllipse(180,100,22,28);
    glColor3f(0.05f,0.04f,0.02f);
    drawSemiEllipse(180,100,14,20);
    glColor3f(0.16f,0.12f,0.08f);
    drawRect(40,90,380,96);
    drawRect(644,90,984,96);
    glColor3f(0.40f,0.34f,0.22f);
    drawLine(40,96,380,96);
    drawLine(644,96,984,96);
    glColor3f(0.85f,0.10f,0.10f);
    drawRect(470,128,554,134);
    glColor3f(1.0f,1.0f,1.0f);
    for (int x=478;x<554;x+=16)
        drawRect(x,128,x+8,134);
    for (int p=0;p<2;p++) {
        float x=(p==0)?467:551;
        drawGradRect(x,110,x+6,145, 0.95f,0.95f,0.95f, 0.65f,0.65f,0.65f);
        glColor3f(0.20f,0.20f,0.20f);
        drawRect(x-1,145,x+7,148);
    }
}

// IRANIAN JET
void drawIranianJet() {
    glPushMatrix();
    glTranslatef(jetIRNx - 974, 0, 0);

    glColor3f(0.50f,0.42f,0.28f);
    glBegin(GL_POLYGON);
        glVertex2f(974,425); glVertex2f(884,432); glVertex2f(854,428);
        glVertex2f(884,422); glVertex2f(974,418);
    glEnd();

    glColor3f(0.30f,0.20f,0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(899,430); glVertex2f(874,428); glVertex2f(884,425); glVertex2f(899,425);
    glEnd();

    glColor3f(0.45f,0.38f,0.25f);
    glBegin(GL_TRIANGLES);
        glVertex2f(939,430); glVertex2f(909,448); glVertex2f(904,432);
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex2f(939,420); glVertex2f(909,402); glVertex2f(904,418);
    glEnd();

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float jetGlow = 0.6f + sin(t*25 + PI)*0.2f;
    drawSoftCircle(974,425,10,1.0f,0.5f,0.2f, jetGlow, 0.0f);
    glDisable(GL_BLEND);

    glPopMatrix();
}

// EXPLOSION
void drawExplosion() {
    float fl = 1.0f + sin(t*9)*0.15f + sin(t*22)*0.05f;
    float sw = shockwave;

    if (sw < 1.0f) {
        float radius = 30 + sw * 180;
        drawRing(512,200,radius-4,radius,1.0f,0.85f,0.4f, 0.7f*(1.0f-sw));
        drawRing(512,200,radius-8,radius-4,1.0f,0.5f,0.15f, 0.4f*(1.0f-sw));
    }

    drawSoftCircle(512,200,90*fl,1.0f,0.55f,0.15f,0.60f,0.0f);
    drawSoftCircle(512,200,60*fl,1.0f,0.40f,0.05f,0.85f,0.0f);
    glColor3f(1.0f,0.35f,0.05f); drawCircle(512,200,32*fl);
    glColor3f(1.0f,0.95f,0.55f); drawCircle(512,200,18*fl);
    glColor3f(1.0f,1.0f,0.85f);  drawCircle(512,200,7*fl);

    if (missileT < 1.0f) {
        float mx = 1000 + (535-1000)*missileT;
        float my = 460 + (215-460)*missileT;
        glColor3f(0.85f,0.85f,0.85f); glLineWidth(2.0f);
        glEnable(GL_LINE_STIPPLE); glLineStipple(2,0x00FF);
        glBegin(GL_LINES);
            glVertex2f(mx + 80, my + 50);
            glVertex2f(mx, my);
        glEnd();
        glDisable(GL_LINE_STIPPLE); glLineWidth(1.0f);

        drawSoftCircle(mx,my,14,1.0f,0.7f,0.3f,0.7f,0.0f);
        glColor3f(1.0f,0.5f,0.2f);
        glBegin(GL_TRIANGLES);
            glVertex2f(mx,my); glVertex2f(mx+12,my+6); glVertex2f(mx+12,my-6);
        glEnd();
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawZone2_Border();
    drawDrones();
    drawSmoke();
    drawExplosion();
    drawIranianJet();

    glutSwapBuffers();
}


void update(int value) {
    t += 0.03f;

    jetIRNx -= 5.15f;
    if (jetIRNx < -100) jetIRNx = 1150;

    for (int i = 0; i < 3; i++) {
        droneUSAx[i] += 0.5f;
        if (droneUSAx[i] > 1100) droneUSAx[i] = -40;
        droneIRNx[i] -= 0.5f;
        if (droneIRNx[i] < -100) droneIRNx[i] = 1100;
    }

    if ((int)(t * 25) % 2 == 0) spawnSmoke();
    for (int i = 0; i < MAX_SMOKE; i++) {
        if (smoke[i].alive) {
            smoke[i].age += 0.03f;
            smoke[i].x += smoke[i].vx;
            smoke[i].y += smoke[i].vy;
            smoke[i].vy *= 0.99f;
            if (smoke[i].age >= smoke[i].life) smoke[i].alive = false;
        }
    }

    float skyTime = fmod(t, 8.0f);
    if (skyTime < 2.0f) skyStage = 0;
    else if (skyTime < 4.0f) skyStage = 1;
    else if (skyTime < 6.0f) skyStage = 2;
    else skyStage = 3;

    missileT += 0.022f;
    if (missileT > 2.8f) missileT = 0.0f;
    if (shockwave < 1.0f) shockwave += 0.028f;
    else shockwave = 0.0f;

    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
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

    for (int i = 0; i < MAX_SMOKE; i++) smoke[i].alive = false;
    srand(42);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Iran-USA Border Scene");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
