/*
    Pinball Game

    Members:
    Prateek Pandey - CED19I057
    Kotamarthi Mohan Himanshu - CED19I026

    to execute: g++ main.cpp -lGL -lGLU -lglut -lGLEW -o prog && ./prog
*/

#include "functions.h"

void Display(){
    glLineWidth(3);
    Point p1={100,100},p2={200,100},p3={200,200},p4={100,200};
    // glColor3f(bdrycolor[0],bdrycolor[1],bdrycolor[2]);
    drawlines(p1,p2);
    drawlines(p3,p2);
    drawlines(p3,p4);
    drawlines(p4,p1);
    printTextStatic();
    // glBegin(GL_QUADS);
    //     glVertex2i(100,100);
    //     glVertex2i(100,200);
    //     glVertex2i(200,200);
    //     glVertex2i(200,100);
    // glEnd();
    glFlush();
}

int main(int argc, char **argv)
{

    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("bdry fill iterative");
    myInit();
    glutDisplayFunc(Display);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}