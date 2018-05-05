#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "SOIL.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 640

#define DIM_X 100
#define DIM_Y 500
#define ROAD_WIDTH 35

using namespace std;

struct GameObject {

    int centerX, centerY;
    int width, height;
};

int getLeft(GameObject);
int getRight(GameObject);
int getDown(GameObject);
int getTop(GameObject);

GameObject road = {DIM_X / 2, DIM_Y / 2, ROAD_WIDTH, DIM_Y};
    
GameObject car, obstacle1, obstacle2, obstacle3;

int roadLineLeft = getLeft(road) + 1;
int roadLineRight = getRight(road) - 1;
int roadLineDown = getDown(road);
int roadLineTop = getTop(road);
int roadLineWidth = 1;

int score, start, rsIndex, FPS, speedIncrease, random, count;
bool obstacle1Active, obstacle2Active, obstacle3Active, stop;

GLuint tex_ID = 0;

int getLeft(GameObject object) {
    return object.centerX - object.width / 2;
}

int getRight(GameObject object) {
    return object.centerX + object.width / 2;
}

int getDown(GameObject object) {
    return object.centerY - object.height / 2;
}

int getTop(GameObject object) {
    return object.centerY + object.height / 2;
}

void drawObject(GameObject object) {

    int left, right, down, top;
    
    left = getLeft(object);
    right = getRight(object);
    down = getDown(object);
    top = getTop(object);

    glBegin(GL_POLYGON);
        glVertex2i(left, down);
        glVertex2i(left, top);
        glVertex2i(right, top);
        glVertex2i(right, down);
    glEnd();
}

bool doesCollide(GameObject object1, GameObject object2) {

    int l1, l2, r1, r2, d1, d2, t1, t2;
    l1 = getLeft(object1);
    r1 = getRight(object1);
    d1 = getDown(object1);
    t1 = getTop(object1);
    
    l2 = getLeft(object2);
    r2 = getRight(object2);
    d2 = getDown(object2);
    t2 = getTop(object2);
    
    if((l1 > r2) || (r1 < l2)) {
        return false;
    }
        
    if((d1 > t2) || (t1 < d2)) {
        return false;
    }
        
    return true;
}

void collisionDetection() {

    if(doesCollide(car, obstacle1) || doesCollide(car, obstacle2) || doesCollide(car, obstacle3)) {
        
        stop = true;
        PlaySound(NULL, NULL, NULL);
        
        cout<<"Score = "<<score<<endl;
        
        
        glColor3f (1.0, 1.0, 0.0);
        char *msg = "Game Over";
        
        int x = DIM_X / 2 - 18;
        int y = DIM_Y / 2;
        
        for(int i=0; i<strlen(msg); i++) {
        
            glRasterPos2d((DIM_X / 2 - 18) + (i * 4) , DIM_Y/2);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
        }
        
        msg = "Score:";
        
        x = DIM_X / 2 - 18;
        y = DIM_Y / 2 - 25;
        
        for(int i=0; i<strlen(msg); i++) {
        
            glRasterPos2d(x + (i * 3) , y);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
        }
        
        int s = score;
        
        x = DIM_X / 2 + 12;
        y = DIM_Y / 2 - 25;
        
        while(s != 0) {
            
            glRasterPos2d(x , y);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ('0' + (s % 10)));
            s = s / 10; 
            x-=3;
        }
        
        msg = "Press 's' to play again and 'q' to quit!";
        
        x = DIM_X / 2 - 40;
        y = DIM_Y / 2 - 50;
        
        for(int i=0; i<strlen(msg); i++) {
        
            glRasterPos2d(x + (i * 2) , y);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg[i]);
        }
    }
}

void timer(int) {

    if(!stop) {
        glutPostRedisplay();
        glutTimerFunc(1000/FPS, timer, 0);
    }
}

void init() {

    car = {40, 50, 6, 40};
    obstacle1 = {40, -50, 6, 30};
    obstacle2 = {50, -50, 6, 30};
    obstacle3 = {60, -50, 6, 30};
    
    obstacle1Active = obstacle2Active = obstacle3Active = stop = false;
    score = start = rsIndex = speedIncrease = random = count = 0;
    FPS = 200;
    
    PlaySound("music.wav", NULL, SND_LOOP|SND_ASYNC);
    glutTimerFunc(0, timer, 0);
}

static void display(void)
{
    
    if(!stop) {
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Drawing road
        glColor3f (0.42, 0.42, 0.42);
        drawObject(road);
        
        // Drawing road side line
        glColor3f(0.8, 0.46, 0.0);
        
        glBegin(GL_POLYGON);
            glVertex2i(roadLineLeft, roadLineDown);
            glVertex2i(roadLineLeft, roadLineTop);
            glVertex2i(roadLineLeft + roadLineWidth, roadLineTop);
            glVertex2i(roadLineLeft + roadLineWidth, roadLineDown);
        glEnd();
        
        glBegin(GL_POLYGON);
            glVertex2i(roadLineRight, roadLineDown);
            glVertex2i(roadLineRight, roadLineTop);
            glVertex2i(roadLineRight - roadLineWidth, roadLineTop);
            glVertex2i(roadLineRight - roadLineWidth, roadLineDown);
        glEnd();
        
        // Drawing road seperator
        glColor3f(0.87, 0.87, 0.87);
        int roadSeperatorWidth = 1;
        
        start--;
        
        if( start < -160) {
            start = 0;
        }
        
        rsIndex = start;
        
        for(int i=1; i<5; i++) {
            
            glBegin(GL_POLYGON);
                glVertex2i(road.centerX - roadSeperatorWidth, rsIndex);
                glVertex2i(road.centerX - roadSeperatorWidth, rsIndex + 130);
                glVertex2i(road.centerX + roadSeperatorWidth, rsIndex + 130);
                glVertex2i(road.centerX + roadSeperatorWidth, rsIndex);
            glEnd();
            
            rsIndex += 160;
        }
        
        // Drawing car
        int left, right, down, top;
        
        if(tex_ID == 0) {
        
            tex_ID = SOIL_load_OGL_texture(
                    "car.bmp",
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
            );
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, tex_ID);
        }
        
        left = getLeft(car);
        right = getRight(car);
        down = getDown(car);
        top = getTop(car);

        glEnable(GL_TEXTURE_2D);
        
        glColor3f (1.0, 1.0, 1.0);
        //drawObject(car);
        
        glBegin(GL_POLYGON);
            glTexCoord2d(0, 0); glVertex2i(left, down);
            glTexCoord2d(0, 1); glVertex2i(left, top);
            glTexCoord2d(1, 1); glVertex2i(right, top);
            glTexCoord2d(1, 0); glVertex2i(right, down);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
        
        count++;
        
        if(count == 150) {
        
            srand ( time(NULL) );
            random = rand() % 3;
            //cout << "Time = " << r<<endl;
            
            if( random == 0) {
                obstacle1Active = true;
            }else if(random == 1) {
                obstacle2Active = true;
            }else if(random == 2) {
                obstacle3Active = true;
            }
            count = 0;
        }
        
        glColor3f (0.38, 1, 0);
        
        if(obstacle1Active) {
        
            drawObject(obstacle1);
            
            obstacle1.centerY = obstacle1.centerY - 1;
            
            if(obstacle1.centerY < 0 ) {
                obstacle1.centerY = DIM_Y;
                obstacle1Active = false;
            }
            
        }
        
        if(obstacle2Active) {
        
            drawObject(obstacle2);
            
            obstacle2.centerY = obstacle2.centerY - 1;
            
            if(obstacle2.centerY < 0 ) {
                obstacle2.centerY = DIM_Y;
                obstacle2Active = false;
            }
            
        }
        
        if(obstacle3Active) {
            
            drawObject(obstacle3);
        
            obstacle3.centerY = obstacle3.centerY - 1;
            
            if(obstacle3.centerY < 0 ) {
                obstacle3.centerY = DIM_Y;
                obstacle3Active = false;
            }
            
        }
        
        // Score
        glColor3f (1.0, 1.0, 0.0);
        score++;
        
        int s = score, x = DIM_X - 5;
        while(s != 0) {
            
            glRasterPos2d(x , DIM_Y - 20);
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ('0' + (s % 10)));
            s = s / 10; 
            x-=3;
        }
        
        // Game Difficulty
        if(speedIncrease != (score / 1000)) {
        
            speedIncrease = score / 1000;
            FPS += speedIncrease;
        }
        
        collisionDetection();
        
        glutSwapBuffers();
    }
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 's':
        case 'S':
            if(stop) {
                init();
            }
            break;
            
        case 'q':
        case 'Q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void catchKey(int key, int x, int y) {

    switch(key) {
        
        case GLUT_KEY_LEFT: 
            
            if(!stop && getLeft(car) > getLeft(road)) {
                car.centerX --;
            }
            
            break;
            
        case GLUT_KEY_RIGHT:
        
            if(!stop && getRight(car) < getRight(road)) {
                car.centerX ++;
            }
            
            break;
    }
    
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(300,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Highway Driving");
    
    init();
    glutDisplayFunc(display);

    glutKeyboardFunc(key);
    glutSpecialFunc(catchKey);
    
    gluOrtho2D(0.0, DIM_X, 0.0, DIM_Y);
    glClearColor(0.35, 0.64, 0.17, 0);
    
    glutMainLoop();

    return EXIT_SUCCESS;
}
