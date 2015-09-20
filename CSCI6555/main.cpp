//
//  main.cpp
//  CSCI6555
//
//  Created by Tan Wang on 9/3/15.
//  Copyright (c) 2015 Tan Wang. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "Matrix.h"
#include "TestClass.h"
#include "Quaternion.h"
// standard
#include <assert.h>
#include <math.h>

#define PI 3.14159265358979

using namespace std;

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;

// position
GLfloat x=0;
GLfloat y=0;
GLfloat z=0;

// keyframe identification
GLint key=0;
GLint maxKey=7;
bool keyInc=true; // see if the key is increasing


//================================
// init
//================================
void init( void ) {
    // init something before main loop...
    TestClass tc;
    int r1=tc.addition(1, 2);
    int r2=tc.mult(2, 3);
    cout << r1 << endl;
    cout << r2 << endl;
}

//================================
// update
//================================
void update( void ) {
    // do something before rendering...
    
    // rotation angle
    g_angle = ( g_angle + 5 ) % 360;
    cout << key << endl;
    
    // switch keyframes
    if(g_angle % 360==0){
        
        /*  Reverse animation, not implement in this assignment
        if(keyInc){
            key++;
            if(key>=maxKey){
                keyInc=false;
                key-=2;
            }
        }
        else{
            key--;
            if(key<0){
                keyInc=true;
                key+=2;
            }
        }*/
        
        if(keyInc){
            key++;
            if(key>=maxKey){
                key=0;
            }
        }

    }
    
}

void put(GLfloat xp, GLfloat yp, GLfloat zp){
    glTranslatef(-x, -y, -z);
    //assert(-z==-222);
    glTranslatef(xp, yp, zp);
    
}



//================================
// render
//================================
void render( void ) {
    // clear buffer
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClearDepth (1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // render state
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // light source attributes
    GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    
    // surface material attributes
    GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
    GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
    GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
    GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
    GLfloat material_Se		= 10;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
    glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
    glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);
    
    // modelview matrix
    glMatrixMode( GL_MODELVIEW );
    
    // Step 1: Get the inputs
    
    // Step 2: Calculate result points
    
    // the position to render
    
    // keyframe testing
    
    // Frames of different objects
    GLfloat frameX[4][7];
    GLfloat frameY[4][7];
    GLfloat frameZ[4][7];
    GLfloat frameXR[4][7]; // fixed angle
    GLfloat frameYR[4][7]; // fixed angle
    GLfloat frameZR[4][7]; // fixed angle
    GLfloat frameQW[4][7]; // quaternion w
    GLfloat frameQA[4][7]; // quaternion a
    GLfloat frameQB[4][7]; // quaternion b
    GLfloat frameQC[4][7]; // quaternion c
    
    for(int i=0; i<4; i++){
        for(int j=0; j<7; j++){
            frameX[i][j]=-2.0f+0.5f*j;
            frameY[i][j]=-1.4f+i*1.0f-sqrt(pow((j-4)*0.125f,2));
            frameZ[i][j]=-5.0f;
            frameQW[i][j]=g_angle/(180.0f/PI);
            if(j%3==0){
                frameXR[i][j]=g_angle/(180.0f/PI);
                frameYR[i][j]=0;
                frameZR[i][j]=0;
                frameQA[i][j]=1;
                frameQB[i][j]=0;
                frameQC[i][j]=0;
            }
            if(j%3==1){
                frameXR[i][j]=0;
                frameYR[i][j]=g_angle/(180.0f/PI);
                frameZR[i][j]=0;
                frameQA[i][j]=0;
                frameQB[i][j]=1;
                frameQC[i][j]=0;
            }

            if(j%3==2){
                frameXR[i][j]=0;
                frameYR[i][j]=0;
                frameZR[i][j]=g_angle/(180.0f/PI);
                frameQA[i][j]=0;
                frameQB[i][j]=0;
                frameQC[i][j]=1;
            }
            

        }
    }
    
    
    
    GLfloat currentX[4];
    GLfloat currentY[4];
    GLfloat currentZ[4];
    GLfloat currentXR[4];
    GLfloat currentYR[4];
    GLfloat currentZR[4];
    GLfloat currentQW[4];
    GLfloat currentQA[4];
    GLfloat currentQB[4];
    GLfloat currentQC[4];
    
    // fixed for now, will implement this using splines
    // i=0, the bottom one, use Catmul-Rom spline with Euler Angles
    // i=1, one above i=0,use B-spline with Euler Angles
    // i=2, one below i=3, use Catmul-Rom spline with Quaternion
    // i=3, the top one, use B-spline with Quaternion
    // The rotation is about x first, then y, then z, and again, in all objects
    // No combination for now
    for(int i=0; i<4; i++){
        currentX[i]=frameX[i][key];
        currentY[i]=frameY[i][key];
        
        currentZ[i]=frameZ[i][key];
        currentXR[i]=frameXR[i][key];
        currentYR[i]=frameYR[i][key];
        currentZR[i]=frameZR[i][key];
        currentQA[i]=frameQA[i][key];
        currentQB[i]=frameQB[i][key];
        currentQC[i]=frameQC[i][key];
        currentQW[i]=frameQW[i][key];
        
    }
    
    for(int i=0; i<4; i++){
        
        // start the render
        glLoadIdentity();
        
        // put the object into the current position
        glTranslatef (currentX[i], currentY[i], currentZ[i]);
        
        GLfloat* r;
        // apply rotations
        if(i<2){
            Quaternion q=Quaternion::fixedAngle(currentXR[i], currentYR[i], currentZR[i]);
            r=q.rMatrix();
        }
        else{
            Quaternion s(currentQW[i],currentQA[i],currentQB[i],currentQC[i],true);
            r=s.rMatrix();
        }
        
        
        
        glMultMatrixf(r);//
        //glRotated(g_angle, 1.0, 1.0, 1.0);
        
        
        
        
        // render objects
        
        glutSolidTeapot(0.125f+0.0625f*(i+1));
        
        
    }
    
    
    // disable lighting
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
    // swap back and front buffers
    glutSwapBuffers();
}




//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
    // screen size
    g_screenWidth  = w;
    g_screenHeight = h;
    
    // viewport
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    
    // projection matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {
    // increase frame index
    g_frameIndex++;
    
    update();
    
    // render
    glutPostRedisplay();
    
    // reset timer
    // 16 ms per frame ( about 60 frames per second )
    glutTimerFunc( 16, timer, 0 );
}

//================================
// main
//================================
int main( int argc, char** argv ) {
    // create opengL window
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
    glutInitWindowSize( 600, 600 );
    glutInitWindowPosition( 100, 100 );
    glutCreateWindow( argv[0] );
    
    // init
    init();
    
    // set callback functions
    glutDisplayFunc( render );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutTimerFunc( 16, timer, 0 );
    
    // main loop
    glutMainLoop();
    
    return 0;
}