#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 660

#define DIM_X 100
#define DIM_Y 500

#define ROAD_WIDTH 35

#define FPS 200

using namespace std;

struct GameObject {

    int centerX, centerY;
    int width, height;
};

int getLeft(GameObject);
int getRight(GameObject);
int getDown(GameObject);
int getTop(GameObject);

GameObject road = { DIM_X / 2, DIM_Y / 2, ROAD_WIDTH, DIM_Y};
    
GameObject car = { 40, 100, 6, 40};

GameObject obstacle1 = {40, -50, 6, 50};

GameObject obstacle2 = {50, -50, 6, 50};

GameObject obstacle3 = {60, -50, 6, 50};

int roadLineLeft = getLeft(road) + 1;
int roadLineRight = getRight(road) - 1;
int roadLineDown = getDown(road);
int roadLineTop = getTop(road);
int roadLineWidth = 1;

int start = 0, rsIndex = 0;
int r = 0;
int count = 0;
bool o1 = true, o2 = false, o3 = false;

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, DIM_X, 0, DIM_Y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

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
        cout<<"Collision"<<endl;
        //exit(0);
    } else {
        cout<<"No collision"<<endl;
    }
}

static void display(void)
{
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
    
    if( start < -120) {
        start = 0;
    }
    
    rsIndex = start;
    
    for(int i=1; i<6; i++) {
        
        glBegin(GL_POLYGON);
            glVertex2i(road.centerX - roadSeperatorWidth, rsIndex);
            glVertex2i(road.centerX - roadSeperatorWidth, rsIndex + 100);
            glVertex2i(road.centerX + roadSeperatorWidth, rsIndex + 100);
            glVertex2i(road.centerX + roadSeperatorWidth, rsIndex);
        glEnd();
        
        rsIndex += 120;
    }
    
    glColor3f (1.0, 0.0, 0.0);
    drawObject(car);
    
    count++;
    
    if(count == 150) {
    
        srand ( time(NULL) );
        r = rand() % 3;
        //cout << "Time = " << r<<endl;
        
        if( r == 0) {
            o1 = true;
        }else if(r == 1) {
            o2 = true;
        }else if(r == 2) {
            o3 = true;
        }
        count = 0;
    }
    
    glColor3f (1.0, 1.0, 0.0);
    
    
    
    if(o1) {
    
        drawObject(obstacle1);
        
        obstacle1.centerY = obstacle1.centerY - 1;
        
        if(obstacle1.centerY < 0 ) {
            obstacle1.centerY = DIM_Y;
            o1 = false;
        }
        
    }
    
    if(o2) {
    
        drawObject(obstacle2);
        
        obstacle2.centerY = obstacle2.centerY - 1;
        
        if(obstacle2.centerY < 0 ) {
            obstacle2.centerY = DIM_Y;
            o2 = false;
        }
        
    }
    
    if(o3) {
        
        drawObject(obstacle3);
    
        obstacle3.centerY = obstacle3.centerY - 1;
        
        if(obstacle3.centerY < 0 ) {
            obstacle3.centerY = DIM_Y;
            o3 = false;
        }
        
    }
    
    collisionDetection();
	    
    glutSwapBuffers();
}

void timer(int) {

    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, 0);
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}

void catchKey(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_LEFT: 
            
            if(getLeft(car) > getLeft(road)) {
                car.centerX --;
            }
            
            break;
            
        case GLUT_KEY_RIGHT:
        
            if(getRight(car) < getRight(road)) {
                car.centerX ++;
            }
            
            break;
    }
    
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Car Driving");

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc(0, timer, 0);
    //glutIdleFunc(idle);
    //glutKeyboardFunc(key);
    glutSpecialFunc(catchKey);
    
    gluOrtho2D(0.0, DIM_X, 0.0, DIM_Y);
    glClearColor(0.35, 0.64, 0.17, 0);
    
    glutMainLoop();

    return EXIT_SUCCESS;
}
