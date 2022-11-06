#include "bits/stdc++.h"
#include "GL/freeglut.h"

using namespace std;
struct Point
{
    int x, y;
};


void myInit(void)
{

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 1000, 0, 1000);
}
void drawlines(Point p1, Point p2)
{
    glBegin(GL_LINES);
    glVertex2i(p1.x, p1.y);
    glVertex2i(p2.x, p2.y);
    glEnd();
}
void mouse(int button, int state, int x, int y)
{

    switch (button)
    {
    case GLUT_RIGHT_BUTTON:
        exit(0);
    default:
        glutIdleFunc(NULL);
        break;
    }
}

void drawpixel(int x,int y){
    // glColor3fv(fillcolor);
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
    glFlush();

}