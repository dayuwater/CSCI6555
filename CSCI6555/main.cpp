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
#include <OpenAL/OpenAL.h>
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
#include "Behavior.h"

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
GLint mode=5;

// Models used for HW3

Teapot* teapot;
Teapot* teapot2;
Teapot* teapot3;
Teapot* teapot4;

Cube* Floor;
Cube* ceiling;
Cube* leftWall;
Cube* rightWall;


Model* world; // abstract model, just use for collision detection


// Models used for HW4 and HW5

Cube* Floor4;
Cube* Ceiling4;
Cube* LeftWall4;
Cube* RightWall4;
Cube* BackWall4;
Cube* FrontWall4;


vector<Teapot*> teapots;
Model* world4;

// models for HW5

Human* m1;
Human* m2;
Human* f1;
Human* f2;




//================================
// init
//================================


void init3(){
    
    
    
    teapot=new Teapot(0.25f,-1.0f,0.0f,-5.0f);
    //teapot->setVelocity(Vec(0,0.5,0.5));
    teapot->setMass(1.0f);
    teapot->addForce(Quaternion(0.1, 1, 0, 0,false)); // regular gravity
    //teapot->addForce(Quaternion(0.02, 1, 0.5, 0.5,false));
    teapot->setVelocity(Vec(0.06,0.06,0));
    teapot->setSpin(0, PI/10, 0);
    teapot->setAccByForce();
    
    
    teapot2=new Teapot(0.25f,1.0f,-0.0f,-5.0f);
    //teapot2->setVelocity(Vec(0,-0.5,0.5));
    teapot2->setMass(1.0f);
    teapot2->addForce(Quaternion(-0.1, 1, 0, 0,false)); // regular gravity
    //teapot2->addForce(Quaternion(0.02, 1, 0.5, 0.5,false));
    teapot2->setVelocity(Vec(-0.06,0.06,0));
    teapot2->setSpin(0, -PI/10, 0);
    teapot2->setAccByForce();
    
    
    teapot3=new Teapot(0.25f,0.0f,1.0f,-5.0f);
    
    teapot3->setMass(1.0f);
    teapot3->setVelocity(Vec(0,0,0));
    teapot3->addForce(Quaternion(-0.1, 0, 1, 0,false)); // regular gravity
    //teapot3->addForce(Quaternion(0.02, 10.5, 0, 0.5,false));
    teapot3->setVelocity(Vec(0.06,-0.06,0));
    teapot3->setSpin(0, PI/10, 0);
    teapot3->setAccByForce();
    
    teapot4=new Teapot(0.25f,-0.0f,-1.0f,-5.0f);
    
    teapot4->setMass(1.0f);
    //teapot4->setVelocity(Vec(0,-10,0));
    teapot4->addForce(Quaternion(0.1, 0, 1, 0,false)); // regular gravity
    //teapot4->addForce(Quaternion(0.02, 1, 0.5, 0.5,false));
    teapot4->setVelocity(Vec(0.06,0.06,0));
    teapot4->setSpin(0, -PI/10, 0);
    teapot4->setAccByForce();
    
    Floor=new Cube(3.0f,0.0f,-3.0f,-5.0f);
    ceiling=new Cube(3.0f,0.0f,3.0f,-5.0f);
    leftWall=new Cube(3.0f,-3.0f,0.0f,-5.0f);
    rightWall=new Cube(3.0f,3.0f,0.0f,-5.0f);
    
    world=new Model();
    // use hierarchial model for the container of all the models for collision detection
    // may implement hierarchial collision detection too
    world->addChild(*Floor);
    
    world->addChild(*ceiling);
    world->addChild(*leftWall);
    world->addChild(*rightWall);
    world->addChild(*teapot);
    world->addChild(*teapot2);
    world->addChild(*teapot3);
    world->addChild(*teapot4);
    
    

    
}

void init4(){
    Floor4=new Cube(9.0f,0.0f,-9.0f,-15.0f);
    Ceiling4=new Cube(9.0f,0.0f,9.0f,-15.0f);
    LeftWall4=new Cube(9.0f,-9.0f,0.0f,-15.0f);
    RightWall4=new Cube(9.0f,9.0f,0.0f,-15.0f);
    BackWall4=new Cube(9.0f,0.0f,0.0f,-24.0f);
    FrontWall4=new Cube(9.0f,0.0f,0.0f,-0.0f);

    // Magic Position: (0,-3.3,-15)
    
    /*
    // x axis
    for(int i=-3.0f; i<=3.0f; i++){
        teapots.push_back(new Teapot(0.25f,i,-3.3f,-15.0f));
    }
    // y axis
    for(int i=-3.3f; i<=5.0f; i++){
        teapots.push_back(new Teapot(0.25f,0.0f,i,-15.0f));
    }
    // z axis
    for(int i=-25.0f; i<=-5.0f; i++){
        teapots.push_back(new Teapot(0.25f,0.0f,-3.3f,i));
    }*/
    
    for(int i=0; i<10; i++){
        
        float x=rand()%400*0.01-2.0f;
        float y=rand()%430*0.01-1.3f;
        float z=rand()%100*0.01-13.0f;
        float vx=rand()%1000*0.001-1.5f;
        float vy=rand()%1000*0.001-1.5f;
        float vz=rand()%1000*0.001-1.5f;
        
    
        teapots.push_back(new Teapot(0.25f,x,y,z));
        teapots[i]->setVelocity(Vec(vx,vy,0.0f));
    }
    
    world4=new Model();
    
    world4->addChild(*Floor4);
    world4->addChild(*Ceiling4);
    world4->addChild(*LeftWall4);
    world4->addChild(*RightWall4);
    world4->addChild(*BackWall4);
    world4->addChild(*FrontWall4);
    for(int i=0; i<teapots.size();i++){
        world4->addChild(*teapots[i]);
        
    }
    
    
}

void init5(){
    Floor4=new Cube(9.0f,0.0f,-9.0f,-15.0f);
    Ceiling4=new Cube(9.0f,0.0f,9.0f,-15.0f);
    LeftWall4=new Cube(9.0f,-9.0f,0.0f,-15.0f);
    RightWall4=new Cube(9.0f,9.0f,0.0f,-15.0f);
    BackWall4=new Cube(9.0f,0.0f,0.0f,-24.0f);
    FrontWall4=new Cube(9.0f,0.0f,0.0f,-0.0f);
    
   

    
    
    
    
    world4=new Model();
    
    world4->addChild(*Floor4);
    world4->addChild(*Ceiling4);
    world4->addChild(*LeftWall4);
    world4->addChild(*RightWall4);
    world4->addChild(*BackWall4);
    world4->addChild(*FrontWall4);
    
    m1=Human::createMale1(0, 0, -10);
    world4->addChild(*m1);
    
    m2=Human::createMale2(0, 1.5, -10);
    world4->addChild(*m2);
    
    f1=Human::createFemale1(1.5, 0, -10);
    world4->addChild(*f1);
    
    
    f2=Human::createFemale2(1.5, 1.5, -10);
    world4->addChild(*f2);

    
    


    
    

    
}
void init( void ) {
    
    srand(time(NULL));
    // init something before main loop...
    TestClass tc;
    int r1=tc.addition(1, 2);
    int r2=tc.mult(2, 3);
    cout << r1 << endl;
    cout << r2 << endl;
    x=-2.0f;
    
    init3();
    //init4();
    init5();
    


    
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
        //teapot->_y=2.0f;
        //teapot2->_y=-2.0f;
        //teapot3->_y=2.0f;
        //teapot4->_y=-2.0f;
        
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
    
    teapot->refresh(0.1f);
    cout << teapot->_y << endl;
    teapot->draw();
    
    
    teapot2->refresh(0.1f);
    //cout << teapot->_y << endl;
    teapot2->draw();
    
    teapot3->refresh(0.1f);
    //cout << teapot->_y << endl;
    teapot3->draw();

    
    teapot4->refresh(0.1f);
    //cout << teapot->_y << endl;
    teapot4->draw();
    
    Floor->draw();
    ceiling->draw();
    leftWall->draw();
    rightWall->draw();
    
    
    
   
    
    
}

void HW4(){
    
    Floor4->draw();
    Ceiling4->draw();
    LeftWall4->draw();
    RightWall4->draw();
    BackWall4->draw();
    for(int i=0; i<teapots.size();i++){
        teapots[i]->refresh(0.1f,2);
        teapots[i]->draw();
    }
    
    
}

void HW5(){
    Floor4->draw();
    Ceiling4->draw();
    LeftWall4->draw();
    RightWall4->draw();
    BackWall4->draw();
    
    
    
   
    m1->refresh(0.5f);
    m1->draw();
    
    m2->refresh(0.5f);
    m2->draw();
    
    f1->refresh(0.5f);
    f1->draw();
    
    f2->refresh(0.5f);
    f2->draw();
    
    
    
    
    glLoadIdentity();
    glTranslatef(1, 0.5, -10);
    glRotatef(PI/2, 1, 0, 0);
    glScalef(0.125, 0.125, 0.125);
    
    glutSolidDodecahedron();

    
    
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
    else if(mode==4){
        HW4();
    }
    else if(mode==5){
        HW5();
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
        case '4':
            mode=4;
            break;
        case '5':
            mode=5;
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