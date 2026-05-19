  #include <GL/freeglut.h>
  #include <cmath>
  #include <cstdlib>

const float PI = 3.14159265f;
float wave = 0.0f;
float zoom = 1.0f;
int winW = 900, winH = 600;

float getWaveOffset(float x)
{
    return 0.02f * sinf(5.0f * x + wave);
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++)
    {
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

void drawStar()
{
    const float S = 0.25f / 175.0f;
    const float LW = 0.014f, RW = 0.006f;

    float tx[5]  = {  0.0f,   123.5f,  76.4f, -76.4f,-123.5f };
    float ty[5]  = {-130.0f,  -40.2f, 105.2f, 105.2f, -40.2f };
    float inX[5] = { 29.2f,   47.2f,   0.0f,  -47.2f, -29.2f };
    float inY[5] = {-40.1f,   15.3f,  49.7f,   15.3f, -40.1f };
    float rX[5]  = { 88.2f,  142.6f,   0.0f, -142.6f, -88.2f };
    float rY[5]  = {-121.3f,  46.4f, 150.0f,   46.4f,-121.3f };

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
    for(float x = -1.0f; x <= 1.0f; x += 0.02f)
    {
        float o1 = getWaveOffset(x), o2 = getWaveOffset(x + 0.02f);
        glVertex2f(x,        y1 + o1); glVertex2f(x + 0.02f, y1 + o2);
        glVertex2f(x + 0.02f,y2 + o2); glVertex2f(x,        y2 + o1);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(zoom, zoom, 1.0f);

    drawFlagStripe( 0.33f,  1.0f,  0.0f, 0.6f, 0.2f);
    drawFlagStripe(-0.33f,  0.33f, 1.0f, 0.8f, 0.0f);
    drawFlagStripe(-1.0f,  -0.33f, 0.8f, 0.0f, 0.0f);

    float cw = getWaveOffset(0.0f);
    glPushMatrix();
    glTranslatef(0.0f, cw, 0.0f);
    glColor3f(0.071f, 0.302f, 0.647f);
    drawCircle(0.0f, 0.0f, 0.25f);
    drawStar();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    winW = w; winH = (h > 0) ? h : 1;
    glViewport(0, 0, winW, winH);
    glutPostRedisplay();
}

void update(int)
{
    wave = wave + 0.06f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int)
{
    switch(key)
    {
        case 'q': case 'Q': exit(0); break;
        case '+': case '=': zoom += 0.1f; break;
        case '-': case '_': zoom -= 0.1f; if(zoom < 0.2f) zoom = 0.2f; break;
    }
 glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("v5 - Complete Flag");
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
