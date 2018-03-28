#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 640

#define DIM_X 50
#define DIM_Y 500

#define ROAD_WIDTH 20

#define FPS 100

using namespace std;

struct GameObject {

    int centerX, centerY;
    int width, height;
};

GameObject road = {
    DIM_X / 2, DIM_Y / 2, 
    ROAD_WIDTH, DIM_Y};
    
GameObject car = { 20, 100, 5, 40};

GameObject obstacle1 = {18, -50, 5, 50};

GameObject obstacle2 = {24, -50, 5, 50};

GameObject obstacle3 = {30, -50, 5, 50};

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

int index = 0;
int rsIndex = 0;

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
        exit(0);
    } else {
        cout<<"No collision"<<endl;
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f (0.67, 0.67, 0.67);
    
    // Drawing road
    drawObject(road);
    
    // Drawing road seperator
    
    /*glColor3f(1, 1, 1);
    int road_seperator_width = 20;
    int road_seperator_height = (WINDOW_HEIGHT - 100) / 3;
    int road_seperator_left = WINDOW_WIDTH/2 - road_seperator_width/2;
    
    for(int i=1; i<=3; i++) {
    
        glBegin(GL_POLYGON);
            glVertex2i(road_seperator_left, 0 - rsIndex);
            glVertex2i(road_seperator_left, road_seperator_height - rsIndex);
            glVertex2i(road_seperator_left + road_seperator_width, road_seperator_height - rsIndex);
            glVertex2i(road_seperator_left + road_seperator_width, 0 - rsIndex);
        glEnd();
    
    }
    
    rsIndex++;*/
    
    glColor3f (1.0, 0.0, 0.0);
    drawObject(car);
    
    count++;
    
    if(count == 150) {
    
        r = rand() % 3;
        cout << r<<endl;
        
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
    glClearColor(0, 1, 0, 0);
    
    glutMainLoop();

    return EXIT_SUCCESS;
}
