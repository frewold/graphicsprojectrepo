#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

float wave = 0;
float zoom = 1.0f;
const float PI = 3.14159265f;

float getWaveOffset(float x)
{
    return 0.02f * sin(5 * x + wave);
}

void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++)
    {
        float theta = 2.0f * PI * i / 100;
        float x = r * cos(theta);
        float y = r * sin(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Draw a thick line as a quad
void drawThickLine(float x1, float y1, float x2, float y2, float w)
{
    float dx = x2 - x1, dy = y2 - y1;
    float len = sqrtf(dx*dx + dy*dy);
    if(len < 0.0001f) return;
    float nx = (-dy / len) * w * 0.5f;
    float ny = ( dx / len) * w * 0.5f;
    glBegin(GL_QUADS);
        glVertex2f(x1 + nx, y1 + ny);
        glVertex2f(x1 - nx, y1 - ny);
        glVertex2f(x2 - nx, y2 - ny);
        glVertex2f(x2 + nx, y2 + ny);
    glEnd();
}

void drawStar()
{
    const float S  = 0.25f / 175.0f;
    const float LW = 0.014f;
    const float RW = 0.006f;

    // Tips (y-flipped so top tip points up in GL space)
    float tipX[5] = {  0.0f*S,  123.5f*S,  76.4f*S, -76.4f*S, -123.5f*S };
    float tipY[5] = { 130.0f*S,  40.2f*S, -105.2f*S, -105.2f*S,  40.2f*S };

    // Inner pentagon corners
    float inX[5]  = {  29.2f*S,  47.2f*S,   0.0f*S, -47.2f*S, -29.2f*S };
    float inY[5]  = {  40.1f*S, -15.3f*S, -49.7f*S, -15.3f*S,  40.1f*S };

    // Ray endpoints
    float rayX[5] = {  88.2f*S,  142.6f*S,   0.0f*S, -142.6f*S,  -88.2f*S };
    float rayY[5] = { 121.3f*S,  -46.4f*S, -150.0f*S,  -46.4f*S,  121.3f*S };

    // Pentagram winding: 0->2->4->1->3->0  (same as canvas preview)
    int order[5] = {0, 2, 4, 1, 3};

    // Step 1: fill pentagram shape with blue (matching canvas fillStyle=blue)
    glColor3f(0.071f, 0.302f, 0.647f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 5; i++)
        glVertex2f(tipX[order[i]], tipY[order[i]]);
    glEnd();

    // Step 2: draw the 5 gold line segments
    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
    {
        int a = order[i], b = order[(i+1)%5];
        drawThickLine(tipX[a], tipY[a], tipX[b], tipY[b], LW);
    }

    // Step 3: sharp miter triangle at each tip to close the pointed gap
    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
    {
        int cur  = order[i];
        int prev = order[(i+4)%5];
        int next = order[(i+1)%5];

        // Unit vectors FROM cur tip along each segment
        float d1x = tipX[prev]-tipX[cur],  d1y = tipY[prev]-tipY[cur];
        float d2x = tipX[next]-tipX[cur],  d2y = tipY[next]-tipY[cur];
        float l1  = sqrtf(d1x*d1x+d1y*d1y);
        float l2  = sqrtf(d2x*d2x+d2y*d2y);
        if(l1 < 0.00001f || l2 < 0.00001f) continue;
        d1x/=l1; d1y/=l1; d2x/=l2; d2y/=l2;

        // Outward normals at the tip for each segment
        float n1x =  d1y*(LW*0.5f),  n1y = -d1x*(LW*0.5f);
        float n2x = -d2y*(LW*0.5f),  n2y =  d2x*(LW*0.5f);

        // Bisector direction (outward)
        float bx = n1x+n2x, by = n1y+n2y;
        float bl = sqrtf(bx*bx+by*by);
        if(bl < 0.00001f) continue;
        bx/=bl; by/=bl;

        // Miter length
        float dot = n1x*bx + n1y*by;
        if(fabsf(dot) < 0.00001f) continue;
        float ml = (LW*0.5f) / dot;

        float miterX = tipX[cur] + bx*ml;
        float miterY = tipY[cur] + by*ml;

        // The two outer quad corners at this tip
        float c1x = tipX[cur] + (-d1y)*(LW*0.5f);
        float c1y = tipY[cur] + ( d1x)*(LW*0.5f);
        float c2x = tipX[cur] + ( d2y)*(LW*0.5f);
        float c2y = tipY[cur] + (-d2x)*(LW*0.5f);

        glBegin(GL_TRIANGLES);
            glVertex2f(miterX, miterY);
            glVertex2f(c1x, c1y);
            glVertex2f(c2x, c2y);
        glEnd();
    }

    // Step 4: repaint inner pentagon blue (covers messy line crossings)
    glColor3f(0.071f, 0.302f, 0.647f);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 5; i++)
        glVertex2f(inX[i], inY[i]);
    glEnd();

    // Step 5: gold rays on top
    glColor3f(1.0f, 0.84f, 0.0f);
    for(int i = 0; i < 5; i++)
        drawThickLine(inX[i], inY[i], rayX[i], rayY[i], RW);
}

void drawFlagStripe(float y1, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    for(float x = -1; x <= 1; x += 0.02f)
    {
        float offset1 = getWaveOffset(x);
        float offset2 = getWaveOffset(x + 0.02f);
        glVertex2f(x,         y1 + offset1);
        glVertex2f(x + 0.02f, y1 + offset2);
        glVertex2f(x + 0.02f, y2 + offset2);
        glVertex2f(x,         y2 + offset1);
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

    float centerWave = getWaveOffset(0.0f);
    glPushMatrix();
    glTranslatef(0.0f, centerWave, 0.0f);

    glColor3f(0.071f, 0.302f, 0.647f);
    drawCircle(0.0f, 0.0f, 0.25f);

    drawStar();

    glPopMatrix();
    glutSwapBuffers();
}

void update(int value)
{
    wave += 0.1f;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'q': case 'Q': exit(0);      break;
        case '+': case '=': zoom += 0.1f; break;
        case '-': case '_':
            zoom -= 0.1f;
            if(zoom < 0.2f) zoom = 0.2f;
            break;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Waving Ethiopian Flag - OpenGL");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
