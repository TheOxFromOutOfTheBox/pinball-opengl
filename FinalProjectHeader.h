#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

#define PI 3.14159265f

float x[] = { -600, -600, -120 }, y[] = { -250, -150, -300 }, X[] = { -600, -600, -120 }, Y[] = { -250, -150, -050 };
float a[] = {600, 600, 120}, b[] = {-250, -150, -300}, A[] = { 600, 600, 120 }, B[] = { -250, -150, -300 };
int score = 0;


class Ball
{
public:
    float ballX = 700, ballY = -300;
    float xMin = -750, xMax = 750, yMin = -375, yMax = 375;
    int ballFlag = 0, bounceCount = 0;
    float xSpeed = 7, ySpeed = 10;
    bool isActive=false;
    void resetBall();
    void launchBall();
    void bounceBall();
    void gravity();
    // For delayed objects
    void drawBoundaries();
} ball;

class Algo
{
public:
    void drawPoint(float x, float y);
    void midPointCircle(float r, float X, float Y);
    void lineDDA(float x0, float y0, float xEnd, float yEnd);
    void edge_detect(float x1a, float y1a, float x2a, float y2a, int *le, int *re);
    void scanFillTriangle(float x1b, float y1b, float x2b, float y2b, float x3b, float y3b);
    void scanFill(float x1b, float y1b, float x2b, float y2b, float x3b, float y3b, float x4b, float y4b);
    void printTextStatic();
} algo;

class Flipper
{
public:
    int rotationFlag = 0;
    int rotCountUp = 0;
    int rotCountDown = 0;
    float angle = 0, theta = 0;
    bool atMax = false;

    void resetFlippers();
    void drawFlippers(float *x, float *y);
    void flipperRotation(float *x, float *y, float *X, float *Y, int flag);

} flipper1, flipper2;

void Ball::resetBall()
{
    ballFlag = 0;
    bounceCount = 0;
    ballX = 700;
    ballY = -300;
    xSpeed = fabs(xSpeed);
    // ySpeed = fabs(ySpeed);
    ySpeed=10;
    isActive=false;
}

void Ball::gravity()
{
    // cout<<ySpeed<<endl;
    if(ballFlag==1 and isActive){
        ySpeed-=0.04;
    }
}
void Ball::launchBall()
{
    if (ballFlag == 1)
    {
        algo.midPointCircle(30, ballX, ballY);
        if (ballY <= 300)
        {
            
            isActive=true;
            ballY += ySpeed;
        }
    }
    else
    {
        algo.midPointCircle(30, ballX, ballY);
        score = 0;
    }
}

void Ball::bounceBall()
{
    algo.midPointCircle(30, ballX, ballY);
    if (bounceCount == 0)
    {
        if (ballX == 700 && ballY >= 300)
        {
            ballX -= xSpeed;
            ballY -= ySpeed;
            ++bounceCount;
        }
    }
    else
    {
        ballX += xSpeed;
        ballY += ySpeed;
        ++bounceCount;
    }

    // Boundary collision
    if (ballX + 30 > xMax)
    {
        ballX = xMax - 30;
        xSpeed = -xSpeed;
        ++bounceCount;
    }
    else if (ballX - 30 < xMin)
    {
        ballX = xMin + 30;
        xSpeed = -xSpeed;
        ++bounceCount;
    }
    if (ballY + 30 > yMax)
    {
        ballY = yMax - 30;
        ySpeed = -ySpeed;
        ++bounceCount;
    }
    else if (ballY - 30 < yMin)
    {
        ballY = yMin + 30;
        ySpeed = -ySpeed;
        ++bounceCount;
    }

    // For blocks beside the flippers
    if (bounceCount)
    {
        if (ballX - 30 <= -600 && ballY - 30 <= -150)
        {
            ySpeed = -ySpeed;
            ++bounceCount;
        }
        if (ballX + 30 >= 600 && ballY - 30 <= -150)
        {
            ySpeed = -ySpeed;
            ++bounceCount;
        }
    }

    // For bumpers in top corners
    if (bounceCount >= 30)
    {
        if (ballX - 30 <= -610 && ballX - 30 >= xMin)
        {
            if (ballY + 30 >= 235 && ballY <= yMax)
            {
                ySpeed = -ySpeed;
                score += 25;
            }
        }
        if (ballX + 30 <= xMax && ballX + 30 >= 610)
        {
            if (ballY + 30 >= 235 && ballY <= yMax)
            {
                ySpeed = -ySpeed;
                score += 25;
            }
        }
    }

    // Game over
    if ((ballY - 30 < -350 && ballX - 30 > X[2] && ballX + 30 < A[2]) ||
        (ballY - 30 < -350))
    {
        resetBall();
        score += 100;
        printf("Score: %d\n", score);
    }
}

void Ball::drawBoundaries()
{
    if (bounceCount)
    {
        glColor3f(0.542, 0.269, 0.074);
        // Left box
        algo.lineDDA(-600, -150, xMin, -150);
        algo.lineDDA(-600, -250, xMin, -250);
        // Right box
        algo.lineDDA(600, -150, xMax, -150);
        algo.lineDDA(600, -250, xMax, -250);

        algo.scanFill(-600, -150, xMin, -150, xMin, -250, -600, -250);
        algo.scanFill(600, -150, xMax, -150, xMax, -250, 600, -250);
    }

    // For bumpers in top corners
    if (bounceCount >= 25)
    {
        glColor3f(1.0, 1.0, 0.0);

        // Left bumper
        algo.lineDDA(xMin, 235, -610, 235);
        algo.lineDDA(-610, 235, -610, yMax);

        // Right bumper
        algo.lineDDA(xMax, 235, 610, 235);
        algo.lineDDA(610, 235, 610, yMax);

        algo.scanFill(xMin, yMax, -610, yMax, xMin, 235, -610, 235);
        algo.scanFill(xMax, yMax, 610, yMax, xMax, 235, 610, 235);
    }
}

/* <----Function definitions---->*/
void Algo::drawPoint(float x, float y)
{
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void Algo::midPointCircle(float r, float X, float Y)
{
    glColor3f(0.621, 0.628, 0.636);
    float x = 0;
    float y = r;
    float decision = 5 / 4 - r;

    while (y > x)
    {
        if (decision < 0)
        {
            x++;
            decision += 2 * x + 1;
        }
        else
        {
            y--;
            x++;
            decision += 2 * (x - y) + 1;
        }
        for (float i = -x; i <= x; i++)
        {
            for (float j = -y; j <= y; j++)
            {
                drawPoint(i + X, j + Y);
                drawPoint(j + X, i + Y);
            }
        }
    }
}

void Algo::lineDDA(float x0, float y0, float xEnd, float yEnd)
{
    float dx = xEnd - x0;
    float dy = yEnd - y0;
    float steps, xIncrement, yIncrement, x = x0, y = y0;

    if (fabs(dx) > fabs(dy))
    {
        steps = fabs(dx);
    }
    else
    {
        steps = fabs(dy);
    }
    xIncrement = GLfloat(dx) / GLfloat(steps);
    yIncrement = GLfloat(dy) / GLfloat(steps);
    drawPoint(round(x), round(y));

    for (int k = 0; k < steps; k++)
    {
        x += xIncrement;
        y += yIncrement;
        drawPoint(round(x), round(y));
    }
}

void Algo::edge_detect(float x1a, float y1a, float x2a, float y2a, int *le, int *re)
{
    float temp, x, mx;

    if (y1a > y2a)
    {
        temp = x1a, x1a = x2a, x2a = temp;
        temp = y1a, y1a = y2a, y2a = temp;
    }

    if (y1a == y2a)
        mx = x2a - x1a;
    else
        mx = (x2a - x1a) / (y2a - y1a);

    x = x1a;

    for (int i = int(y1a) + abs(ball.yMin); i <= (int)y2a + abs(ball.yMin); i++)
    {
        if (x < (float)le[i])
            le[i] = (int)x;
        if (x > (float)re[i])
            re[i] = (int)x;
        x += mx;
    }
}

void Algo::scanFillTriangle(float x1b, float y1b, float x2b, float y2b, float x3b, float y3b)
{
    int xis = abs(ball.yMin) + abs(ball.yMax) + 1;
    int le[900], re[900];

    for (int i = 0; i < ball.yMax - ball.yMin + 1; i++)
        le[i] = ball.xMax;
    for (int j = 0; j < ball.yMax - ball.yMin + 1; j++)
        re[j] = ball.xMin;

     edge_detect(x1b, y1b, x2b, y2b, le, re);
     edge_detect(x2b, y2b, x3b, y3b, le, re);
     edge_detect(x3b, y3b, x1b, y1b, le, re);

    for (int j = 0; j < ball.yMax - ball.yMin + 1; j++)
    {
        if (le[j] <= re[j])
            for (int i = le[j]; i < re[j]; i++)
                drawPoint(i, j - abs(ball.yMin));
    }
}

void Algo::scanFill(float x1b, float y1b, float x2b, float y2b, float x3b, float y3b, float x4b, float y4b)
{
    int le[900], re[900];

    for (int i = 0; i < ball.yMax - ball.yMin + 1; i++)
        le[i] = ball.xMax;
    for (int j = 0; j < ball.yMax - ball.yMin + 1; j++)
        re[j] = ball.xMin;

     edge_detect(x1b, y1b, x2b, y2b, le, re);
     edge_detect(x2b, y2b, x3b, y3b, le, re);
     edge_detect(x3b, y3b, x4b, y4b, le, re);
     edge_detect(x4b, y4b, x1b, y1b, le, re);

    for (int j = 0; j < ball.yMax - ball.yMin + 1; j++)
    {
        if (le[j] <= re[j])
            for (int i = le[j]; i < re[j]; i++)
                drawPoint(i, j - abs(ball.yMin));
    }
}


    void Algo::printTextStatic()
    {
        // cout<<"hi"<<endl;
        string score_txt = "SCORE : ";
        // string tries = "TRIES LEFT : ";

        string instructions1 = "Press SPACEBAR to Start, ESC to Exit";
        string instructions2 = "Press Z for left flipper, M for right flipper.";
        if (!ball.isActive)
        {
            glRasterPos2i(-240, 290);
            for (int i = 0; i < instructions1.size(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)instructions1[i]);
            }
            glRasterPos2i(-240, 260);
            for (int i = 0; i < instructions2.size(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)instructions2[i]);
            }
        }
        else
        {
            glRasterPos2i(-50, 320);
            for (int i = 0; i < score_txt.size(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)score_txt[i]);
            }
            string text_score = to_string(score);
            for (int i = 0; i < text_score.size(); i++)
            {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text_score[i]);
            }
        }
        // glRasterPos2i(800, 900);
        // for (int i = 0; i < tries.size(); i++)
        // {
        //     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)tries[i]);
        // }
        // string text_tries = to_string(tries_left);
        // for (int i = 0; i < text_tries.size(); i++)
        // {
        //     glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text_tries[i]);
        // }
    }


void Flipper::resetFlippers()
{
    rotationFlag = 0;
    atMax = false;
    angle = 0;
    rotCountUp = 0;
    rotCountDown = 0;
}

void Flipper::drawFlippers(float *x, float *y)
{
    glColor3f(0.878, 0, 0.324);

    // Left flipper
    algo.lineDDA(x[0], y[0], x[1], y[1]);
    algo.lineDDA(x[1], y[1], x[2], y[2]);
    algo.lineDDA(x[2], y[2], x[0], y[0]);

    algo.scanFillTriangle(x[0], y[0], x[1], y[1], x[2], y[2]);
}


void Flipper::flipperRotation(float *x, float *y, float *X, float *Y,int flag)
{
    if (rotationFlag == 1)
    {
        glColor3f(0.578, 0, 0.824);
        if (flag==0) // Flag=0 For left flipper Flag = 1 Right flipper
        {
            theta =(angle * PI / 180);
            for (int i = 0; i < 3; i++)
            {
                X[i] = ((x[i] + 600) * cos(theta)) - ((y[i] + 200) * sin(theta)) - 600;
                Y[i] = ((x[i] + 600) * sin(theta)) + ((y[i] + 200) * cos(theta)) - 200;
            }
        }
        else
        {//right flipper
            theta =-(angle * PI / 180);
            for (int i = 0; i < 3; i++)
            {
                X[i] = ((x[i] - 600) * cos(theta)) - ((y[i] + 200) * sin(theta)) + 600 ;
                Y[i] = ((x[i] - 600) * sin(theta)) + ((y[i] + 200) * cos(theta)) - 200;
            }
        }
        
        algo.lineDDA(X[0], Y[0], X[1], Y[1]);
        algo.lineDDA(X[1], Y[1], X[2], Y[2]);
        algo.lineDDA(X[2], Y[2], X[0], Y[0]);
        algo.scanFillTriangle(X[0], Y[0], X[1], Y[1], X[2], Y[2]);

        // Collision detection
        cout << ball.bounceCount << endl;
        if (ball.bounceCount)
        {
            //cout<<flag<<" "<<X[2]<<" "<<(ball.ballX)<<" "<<X[0]<<endl;
            if(flag==0)
            {
                if ((ball.ballX - 30 <= X[2] && ball.ballX - 30 >= X[0]))
                {
                    if ((ball.ballY - 30 <= Y[2] && ball.ballY - 30 >= Y[0]) or (ball.ballY - 30 >= Y[2] && ball.ballY - 30 <= Y[0]))
                    {
                        ball.ySpeed = -ball.ySpeed;
                    }
                }
            }
            else
            {
                if ((ball.ballX - 30 >= X[2] && ball.ballX - 30 <= X[0]))
                {
                    if ((ball.ballY - 30 <= Y[2] && ball.ballY - 30 >= Y[0]) or (ball.ballY - 30 >= Y[2] && ball.ballY - 30 <= Y[0]))
                    {
                        ball.ySpeed = -ball.ySpeed;
                    }
                }
            }
        }

        if (angle <= 30)
        {
            ++rotCountUp;
            if (rotCountUp >= 40)
            {
                atMax = true;
            }
            else
            {
                angle += 0.75;
            }
        }

        if (atMax)
        {
            if (flag==0)//left flipper
            {
                theta =(angle * PI / 180);
                for (int i = 0; i < 3; i++)
                {
                    X[i] = ((x[i] + 600) * cos(theta)) - ((y[i] + 200) * sin(theta)) - 600;
                    Y[i] = ((x[i] + 600) * sin(theta)) + ((y[i] + 200) * cos(theta)) - 200;
                }
            }
        else
        {//right flipper
            theta =-(angle * PI / 180);
            for (int i = 0; i < 3; i++)
            {
                X[i] = ((x[i] - 600) * cos(theta)) - ((y[i] + 200) * sin(theta)) + 600 ;
                Y[i] = ((x[i] - 600) * sin(theta)) + ((y[i] + 200) * cos(theta)) - 200;

            }
        }
            algo.lineDDA(X[0], Y[0], X[1], Y[1]);
            algo.lineDDA(X[1], Y[1], X[2], Y[2]);
            algo.lineDDA(X[2], Y[2], X[0], Y[0]);

            if (angle >= 0)
            {
                ++rotCountDown;
                if (rotCountDown >= 40)
                {
                    resetFlippers();
                }
                else
                {
                    angle -= 0.75;
                }
            }
        }
    }
    else
    {
        drawFlippers(x, y);
        //cout << "HERE IN FLSE" << endl;
        // Collision detection
        //cout << ball.bounceCount << endl;
        if (ball.bounceCount)
        {
            cout << "Passed this" << endl;
            //cout << flag << " " << x[2] << " " << (ball.ballX) << " " << x[0] << endl;
            cout << X[0] << endl;
            if (flag == 0)
            {
                if ((ball.ballX - 30 <= X[2] && ball.ballX - 30 >= X[0]))
                {
                    if ((ball.ballY - 30 >= Y[2] && ball.ballY - 30 <= Y[0]))
                    {
                        ball.ySpeed = -ball.ySpeed;
                    }
                }
            }
            else {
                if ((ball.ballX + 30 >= X[2] && ball.ballX + 30 <= X[0]))
                {
                    if ((ball.ballY - 30 >= Y[2] && ball.ballY - 30 <= Y[0]))
                    {
                        ball.ySpeed = -ball.ySpeed;
                    }
                }
            }
        
        }
    }
}
