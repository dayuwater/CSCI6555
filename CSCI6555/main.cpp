//
//  main.cpp
//  CSCI6555
//
//  Created by Tan Wang on 9/3/15.
//  Copyright (c) 2015 Tan Wang. All rights reserved.
//
// Acknowledgement:
// All the files in resources folder are the reused files I used in CSCI 4554
// I use them in this project for geometric data of models

#include <iostream>
#include <fstream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "Matrix.h"
#include "TestClass.h"
#include "Quaternion.h"
#include "Splines.h"
// standard
#include <assert.h>
#include <math.h>
#include "HW1.cpp"
#include "HW2.cpp"
#include "Models.h"
#include "Util.h"

#define PI 3.14159265358979
#define RESOURCE_DIR   "/Users/tanwang/Documents/CSCI6555/CSCI6555/resources/"



using namespace std;
using namespace Util;
using namespace Util::Constants;

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 30;

// position
float x=0;
float y=0;
float z=0;

// position in last frame
float px=0;
float py=0;
float pz=0;


// keyframe identification
GLint key=0;
GLint maxKey=7;
bool keyInc=true; // see if the key is increasing

// Modes:
// 1=HW1
// 2=HW2
// Going to use one project for all homeworks
GLint mode=2;



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
    x=-2.0f;
}

//================================
// update
//================================
void update( void ) {
    // do something before rendering...
    
    // rotation angle
    g_angle = ( g_angle + 2 ) % 360;
    //cout << key << endl;
    
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
            if(key>=maxKey-3){
                key=0;
            }
        }
        
    }
    x+=0.005f;
    if(x>=2.0f){
        x=-2.0f;
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

// render help functions
void renderReady(){
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
    
    

    
}


void HW3(){
    Teapot c;
    c.draw();
    
}



void render( void ) {
    renderReady();
    if(mode==1){
        HW1(g_angle,key,maxKey);
    }
    else if(mode==2){
        HW2(g_angle,key,maxKey,x,y,z,px,py,pz);
    }
    else if(mode==3){
        HW3();
    }
    
    glutSwapBuffers();
}




//================================
// keyboard input
//================================
void keyboard( unsigned char key, int x, int y ) {
    switch(key){
        case '1':
            mode=1;
            break;
        case '2':
            mode=2;
            break;
        case '3':
            mode=3;
            break;
    }
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