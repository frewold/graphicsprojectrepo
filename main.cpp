#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <vector>

const float PI = 3.14159265f;
float wave = 0.0f;
int activeObj = 0;

struct Transform { float tx, ty, angle, sx, sy; };
Transform flagT = { -0.5f, 0.0f, 0.0f, 0.38f, 0.38f };
Transform nikeT = {  0.5f, 0.0f, 0.0f, 0.55f, 0.55f };

int winW = 1200, winH = 600;

float getWaveOffset(float x)
{
    return 0.02f * sinf(5.0f * x + wave);
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++) {
        float th = 2.0f * PI * i / 100.0f;
        glVertex2f(cx + r * cosf(th), cy + r * sinf(th));
    }
    glEnd();
}

void drawThickLine(float x1, float y1, float x2, float y2, float w)
{
    float dx = x2-x1, dy = y2-y1;
    float len = sqrtf(dx*dx + dy*dy);
    if(len < 0.0001f) return;
    float nx = (-dy/len)*w*0.5f, ny = (dx/len)*w*0.5f;
    glBegin(GL_QUADS);
        glVertex2f(x1+nx, y1+ny); glVertex2f(x1-nx, y1-ny);
        glVertex2f(x2-nx, y2-ny); glVertex2f(x2+nx, y2+ny);
    glEnd();
}

void drawTipTri(float tipX, float tipY, float bx1, float by1, float bx2, float by2)
{
    glBegin(GL_TRIANGLES);
        glVertex2f(tipX, tipY);
        glVertex2f(bx1,  by1);
        glVertex2f(bx2,  by2);
    glEnd();
}

void drawStar(float scaleHint)
{
    const float S  = 0.25f / 175.0f;
    float tx[5]  = {  0.0f,   123.5f,  76.4f, -76.4f,-123.5f };
    float ty[5]  = {-130.0f,  -40.2f, 105.2f, 105.2f, -40.2f };
    float inX[5] = { 29.2f,   47.2f,   0.0f,  -47.2f, -29.2f };
    float inY[5] = {-40.1f,   15.3f,  49.7f,   15.3f, -40.1f };
    float rX[5]  = { 88.2f,  142.6f,   0.0f, -142.6f, -88.2f };
    float rY[5]  = {-121.3f,  46.4f, 150.0f,   46.4f,-121.3f };

    float safeScale = (scaleHint > 0.50f) ? scaleHint : 0.50f;
    float LW = 0.014f / safeScale;
    float RW = 0.006f / safeScale;

    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
    {
        int j = (i+2)%5;
        drawThickLine(tx[i]*S, -ty[i]*S, tx[j]*S, -ty[j]*S, LW);
    }

    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
    {
        float tX = tx[i]*S, tY = -ty[i]*S;
        int ia = (i+2)%5, ib = (i+3)%5;
        float ax = tx[ia]*S - tX, ay = -ty[ia]*S - tY;
        float bx = tx[ib]*S - tX, by = -ty[ib]*S - tY;
        float lA = sqrtf(ax*ax+ay*ay), lB = sqrtf(bx*bx+by*by);
        if(lA < 0.00001f || lB < 0.00001f) continue;
        ax/=lA; ay/=lA; bx/=lB; by/=lB;
        float oAx =  ay*(LW*0.5f), oAy = -ax*(LW*0.5f);
        float oBx = -by*(LW*0.5f), oBy =  bx*(LW*0.5f);
        float bisx = oAx+oBx, bisy = oAy+oBy;
        float bL = sqrtf(bisx*bisx+bisy*bisy);
        if(bL < 0.00001f) continue;
        bisx/=bL; bisy/=bL;
        float dot = oAx*bisx + oAy*bisy;
        if(fabsf(dot) < 0.00001f) continue;
        float mL = (LW*0.5f)/dot;
        drawTipTri(tX+bisx*mL, tY+bisy*mL, tX+oAx, tY+oAy, tX+oBx, tY+oBy);
    }

    glColor3f(0.071f, 0.302f, 0.647f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 5; i++) glVertex2f(inX[i]*S, -inY[i]*S);
    glEnd();

    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
        drawThickLine(inX[i]*S, -inY[i]*S, rX[i]*S, -rY[i]*S, RW);
}

void drawFlagStripe(float y1, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    for(float x = -1.0f; x <= 1.0f; x += 0.02f) {
        float o1 = getWaveOffset(x), o2 = getWaveOffset(x + 0.02f);
        glVertex2f(x,        y1+o1); glVertex2f(x+0.02f, y1+o2);
        glVertex2f(x+0.02f,  y2+o2); glVertex2f(x,       y2+o1);
    }
    glEnd();
}

void drawFlag()
{
    glPushMatrix();
    glTranslatef(flagT.tx, flagT.ty, 0.0f);
    glRotatef(flagT.angle, 0.0f, 0.0f, 1.0f);
    glScalef(flagT.sx, flagT.sy, 1.0f);

    drawFlagStripe( 0.33f,  1.0f,  0.0f, 0.6f, 0.2f);
    drawFlagStripe(-0.33f,  0.33f, 1.0f, 0.8f, 0.0f);
    drawFlagStripe(-1.0f,  -0.33f, 0.8f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, getWaveOffset(0.0f), 0.0f);
    glColor3f(0.071f, 0.302f, 0.647f);
    drawCircle(0.0f, 0.0f, 0.25f);
    drawStar(flagT.sx);
    glPopMatrix();

    glPopMatrix();
}

struct Pt { float x, y; };

void bez(std::vector<Pt>& v,
         float x0, float y0, float x1, float y1,
         float x2, float y2, float x3, float y3, int n = 50)
{
    for(int i = 0; i <= n; i++) {
        float t = (float)i/n, u = 1.0f-t;
        float u3=u*u*u, u2t=3*u*u*t, ut2=3*u*t*t, t3=t*t*t;
        v.push_back({u3*x0+u2t*x1+ut2*x2+t3*x3,
                     u3*y0+u2t*y1+ut2*y2+t3*y3});
    }
}

void drawNike()
{
    glPushMatrix();
    glTranslatef(nikeT.tx, nikeT.ty, 0.0f);
    glRotatef(nikeT.angle, 0.0f, 0.0f, 1.0f);
    glScalef(nikeT.sx, nikeT.sy, 1.0f);

    glScalef(1.0f/150.0f, 1.0f/150.0f, 1.0f);
    glTranslatef(-75.0f, 50.0f, 0.0f);
    glScalef(1.0f, -1.0f, 1.0f);

    float cx = 42.741f, cy = 71.477f;
    std::vector<Pt> bot, top;

    bez(bot,cx,cy, cx-9.881f,cy+11.604f, cx-19.355f,cy+25.994f, cx-19.45f,cy+36.75f);
    cx-=19.45f; cy+=36.75f;
    bez(bot,cx,cy, cx-0.037f,cy+4.047f, cx+1.255f,cy+7.58f, cx+4.354f,cy+10.256f);
    cx+=4.354f; cy+=10.256f;
    bez(bot,cx,cy, cx+4.46f,cy+3.854f, cx+9.374f,cy+5.213f, cx+14.264f,cy+5.221f);
    cx+=14.264f; cy+=5.221f;
    bez(bot,cx,cy, cx+7.146f,cy+0.01f, cx+14.242f,cy-2.873f, cx+19.798f,cy-5.096f);
    cx+=19.798f; cy-=5.096f;
    bez(bot,cx,cy, cx+9.357f,cy-3.742f, cx+112.79f,cy-48.659f, cx+112.79f,cy-48.659f);
    cx+=112.79f; cy-=48.659f;

    bez(top,cx,cy, cx+0.998f,cy-0.5f, cx+0.811f,cy-1.123f, cx-0.438f,cy-0.812f);
    cx-=0.438f; cy-=0.812f;
    bez(top,cx,cy, cx-0.504f,cy+0.126f, cx-112.603f,cy+30.505f, cx-112.603f,cy+30.505f);
    cx-=112.603f; cy+=30.505f;
    cx-=6.524f; cy+=0.934f;
    top.push_back({cx, cy});
    bez(top,cx,cy, cx-8.615f,cy+0.051f, cx-16.281f,cy-4.731f, cx-16.219f,cy-14.808f);
    cx-=16.219f; cy-=14.808f;
    bez(top,cx,cy, cx+0.024f,cy-3.943f, cx+1.231f,cy-8.698f, cx+4.028f,cy-14.291f);

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUAD_STRIP);
    size_t bS = bot.size(), tS = top.size(), st = (bS > tS) ? bS : tS;
    for(size_t i = 0; i < st; i++) {
        size_t bi = (i*(bS-1))/(st-1);
        size_t ti = (st-1-i)*(tS-1)/(st-1);
        glVertex2f(bot[bi].x, bot[bi].y);
        glVertex2f(top[ti].x, top[ti].y);
    }
    glEnd();

    glPopMatrix();
}

void drawDivider()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.45f, 0.45f, 0.45f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
        glVertex2f((float)winW*0.5f, 0.0f);
        glVertex2f((float)winW*0.5f, (float)winH);
    glEnd();
    glLineWidth(1.0f);

    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, winW, winH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float asp = (float)winW / (float)winH;
    gluOrtho2D(-asp, asp, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawFlag();
    drawNike();
    drawDivider();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    winW = w;
    winH = (h > 0) ? h : 1;
    glutPostRedisplay();
}

void update(int)
{
    wave += 0.06f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int)
{
    Transform& t = (activeObj == 0) ? flagT : nikeT;
    const float dT = 0.05f, dA = 5.0f, dS = 0.05f;
    switch(key)
    {
        case 'w': case 'W': t.ty += dT; break;
        case 's': case 'S': t.ty -= dT; break;
        case 'a': case 'A': t.tx -= dT; break;
        case 'd': case 'D': t.tx += dT; break;
        case 'e': case 'E': t.angle += dA; break;
        case 'c': case 'C': t.angle -= dA; break;
        case '+': case '=': t.sx += dS; t.sy += dS; break;
        case '-': case '_':
            t.sx -= dS; t.sy -= dS;
            if(activeObj == 0) { if(t.sx < 0.50f) { t.sx = 0.50f; t.sy = 0.50f; } }
            else               { if(t.sx < 0.05f) { t.sx = 0.05f; t.sy = 0.05f; } }
            break;
        case '\t': activeObj = 1 - activeObj; break;
        case 'r': case 'R':
            flagT = { -0.5f, 0.0f, 0.0f, 0.38f, 0.38f };
            nikeT = {  0.5f, 0.0f, 0.0f, 0.55f, 0.55f };
            break;
        case 'q': case 'Q': exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int)
{
    Transform& t = (activeObj == 0) ? flagT : nikeT;
    const float dT = 0.05f;
    switch(key) {
        case GLUT_KEY_UP:    t.ty += dT; break;
        case GLUT_KEY_DOWN:  t.ty -= dT; break;
        case GLUT_KEY_LEFT:  t.tx -= dT; break;
        case GLUT_KEY_RIGHT: t.tx += dT; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Ethiopian Flag  |  Nike Swoosh");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
