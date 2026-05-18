#include <GL/freeglut.h>
#include <cmath>

const float PI = 3.14159265f;
float wave = 0.0f;
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

    drawFlagStripe( 0.33f,  1.0f,  0.0f, 0.6f, 0.2f);
    drawFlagStripe(-0.33f,  0.33f, 1.0f, 0.8f, 0.0f);
    drawFlagStripe(-1.0f,  -0.33f, 0.8f, 0.0f, 0.0f);

    float cw = getWaveOffset(0.0f);
    glPushMatrix();
    glTranslatef(0.0f, cw, 0.0f);
    glColor3f(0.071f, 0.302f, 0.647f);
    drawCircle(0.0f, 0.0f, 0.25f);
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
    wave += 0.06f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("v3 - Flag + Circle");
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
