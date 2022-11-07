#include "bits/stdc++.h"
#include "GL/freeglut.h"

using namespace std;

int score=0,tries_left=3;

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

// mouse functions
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

void drawpixel(int x, int y)
{
    // glColor3fv(fillcolor);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// circle functions
void DrawCirclePoint(int x, int y, int xc, int yc)
{
    glPointSize(1.5);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2i(x + xc, y + yc);
    glVertex2i(-x + xc, y + yc);
    glVertex2i(x + xc, -y + yc);
    glVertex2i(-x + xc, -y + yc);
    glVertex2i(y + xc, x + yc);
    glVertex2i(y + xc, -x + yc);
    glVertex2i(-y + xc, x + yc);
    glVertex2i(-y + xc, -x + yc);
    glEnd();
}

void midptCircleDrawing(int xc, int yc, int r)
{
    int p0 = 1 - r;
    int x = r;
    int y = 0;
    int pk = p0;
    DrawCirclePoint(x, y, xc, yc);
    while (x > y)
    {
        if (pk < 0)
        {
            y++;
            pk += (2 * y) + 1;
        }
        else
        {
            y++;
            x--;
            pk -= 2 * (x - y);
        }
        DrawCirclePoint(x, y, xc, yc);
    }
}

void bresenhamLineDrawingAlgo(Point p1, Point p2)
{
    int x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    int dx, dy, i, P;
    int incx, incy, inc1, inc2;

    dx = x2 - x1;
    dy = y2 - y1;

    // making abs..
    dx = abs(dx);
    dy = abs(dy);

    incx = 1;
    if (x2 < x1)
    {
        incx = -1;
    }

    incy = 1;
    if (y2 < y1)
    {
        incy = -1;
    }

    int x = x1;
    int y = y1;

    // m <= 1
    if (dx > dy)
    {
        drawpixel(x, y);
        // e = Po
        P = 2 * dy - dx;
        inc1 = 2 * (dy - dx);
        inc2 = 2 * dy;
        for (i = 0; i < dx; i++)
        {
            if (P >= 0)
            {
                y += incy;
                P += inc1;
            }
            else
                P += inc2;

            x += incx;
            drawpixel(x, y);
        }
    }
    else
    {
        drawpixel(x, y);
        P = 2 * dx - dy;
        inc1 = 2 * (dx - dy);
        inc2 = 2 * dx;
        for (i = 0; i < dy; i++)
        {
            if (P >= 0)
            {
                x += incx;
                P += inc1;
            }
            else
                P += inc2;
            y += incy;
            drawpixel(x, y);
        }
    }
}


void printTextStatic(){
    string score_txt="SCORE : ";
    string tries="TRIES LEFT : ";

    glRasterPos2i(800,950);
    for(int i=0;i<score_txt.size();i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int) score_txt[i]);
    }
    string text_score=to_string(score);
    for(int i=0;i<text_score.size();i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int) text_score[i]);
    }
    glRasterPos2i(800,900);
    for(int i=0;i<tries.size();i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int) tries[i]);
    }
    string text_tries=to_string(tries_left);
    for(int i=0;i<text_tries.size();i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int) text_tries[i]);
    }
}

void drawObjects(){
    
}