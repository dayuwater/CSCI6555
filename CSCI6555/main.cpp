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



void HW2(){
    
    // Frames of different objects
    GLfloat frameX[7];
    GLfloat frameY[7];
    GLfloat frameZ[7];
    
    
    for(int j=0; j<7; j++){
        frameX[j]=0.0f+2.0f*cosf(PI/3*j);
        frameY[j]=0.5f;
        frameZ[j]=-1.0f+2.0f*sinf(PI/3*j);
        
        
    }
    
    Animations::CatmulRomSpline cat;
    for(int i=0; i<7; i++){
        cat.addPosWithAngle(frameX[i], frameY[i], frameZ[i], 0, 0, 0);
    }
    
    

    
   // Initialize The Ground
    Cube c(4.0f,-0.0f,-2.0f,-2.0f);
    c._rx=g_angle/59.0f;
    c._qw=0.0f;
    c._qa=1.0f;
    c._qb=1.0f;
    c.draw(1);
    
    // Initialize The models
    
    PolyModel* torso=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model_file = RESOURCE_DIR + string("torso.d2");
    ifstream car_fs(model_file);
    torso->_ry=0.0f;
    torso->loadModel(car_fs);
    torso->scale(0.25f);
    torso->translate(0.0f, 1.0f, 0.0f,false);
    
    
    PolyModel* leg1=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model2_file = RESOURCE_DIR + string("torso.d2");
    ifstream car2_fs(model2_file);
    leg1->_ry=0.0f;
    leg1->loadModel(car2_fs);
   
    torso->addPolyChild(*leg1);
    leg1->scale(0.125f,0.5f,0.125f);
    
    leg1->translateToParent(0.0f, 0.0f, 0.0f);
    {
        Vec a(leg1->getCenter().x(),leg1->getMinVert().y(),leg1->getCenter().z());
        leg1->attach=a;
        Vec b(torso->getMinVert().x(),torso->getMinVert().y(),torso->getMinVert().z());
        leg1->parentAttach=b;
    }
   
    

    
    
    PolyModel* leg2=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model3_file = RESOURCE_DIR + string("torso.d2");
    ifstream car3_fs(model3_file);
    leg2->_ry=0.0f;
    leg2->loadModel(car3_fs);
    
    torso->addPolyChild(*leg2);
    leg2->scale(0.125f,0.5f,0.125f);
   
    leg2->translateToParent(0.0f,0.0f,0.0f);
    {
        Vec a(leg2->getCenter().x(),leg2->getMinVert().y(),leg2->getCenter().z());
        leg2->attach=a;
        Vec b(torso->getMaxVert().x(),torso->getMinVert().y(),torso->getMaxVert().z());
        leg2->parentAttach=b;
    }
    
    PolyModel* arm1=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model4_file = RESOURCE_DIR + string("torso.d2");
    ifstream car4_fs(model4_file);
    arm1->_ry=0.0f;
    arm1->loadModel(car4_fs);
    
    torso->addPolyChild(*arm1);
    arm1->scale(0.125f,0.5f,0.125f);
    
    arm1->translateToParent(0.0f,0.0f,0.0f);
    {
        Vec a(arm1->getMaxVert().x(),arm1->getMaxVert().y(),arm1->getCenter().z());
        arm1->attach=a;
        Vec b(torso->getMinVert().x(),torso->getMaxVert().y(),torso->getMinVert().z());
        arm1->parentAttach=b;
    }
    
    PolyModel* arm2=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model5_file = RESOURCE_DIR + string("torso.d2");
    ifstream car5_fs(model5_file);
    arm2->_ry=0.0f;
    arm2->loadModel(car5_fs);
    
    torso->addPolyChild(*arm2);
    arm2->scale(0.125f,0.5f,0.125f);
    
    arm2->translateToParent(0.0f,0.0f,0.0f);
    {
        Vec a(arm2->getMinVert().x(),arm2->getMaxVert().y(),arm2->getCenter().z());
        arm2->attach=a;
        Vec b(torso->getMaxVert().x(),torso->getMaxVert().y(),torso->getMaxVert().z());
        arm2->parentAttach=b;
    }
    
    PolyModel* head=new PolyModel(POSITION.x(),POSITION.y(),POSITION.z());
    string model6_file = RESOURCE_DIR + string("torso.d2");
    ifstream car6_fs(model6_file);
    head->_ry=0.0f;
    head->loadModel(car6_fs);
    
    torso->addPolyChild(*head);
    head->scale(0.125f,0.125f,0.125f);
    
    head->translateToParent(0.0f,0.0f,0.0f);
    {
        Vec a(head->getCenter().x(),head->getMinVert().y(),head->getCenter().z());
        head->attach=a;
        Vec b(torso->getCenter().x(),torso->getMaxVert().y(),torso->getMaxVert().z());
        head->parentAttach=b;
    }




    
    // Translate the models
    
    vector<PolyModel*> d=torso->getPolyDecendents();
    
    /*
      Passenger Mover Code
    if(x<-0.5f||x>0.5f){
        torso->translate(x, y, z,false);
    }
    else {
        torso->translate(x, y, z);
    }*/
    float rAngle=Util::rotateFunction(g_angle/59.0f);
    float rAngle2=Util::rotateFunction(g_angle/59.0f,1);
    leg1->rotate2(rAngle);
    leg2->rotate2(rAngle2);
    arm1->rotate2(rAngle2);
    arm2->rotate2(rAngle);
    head->rotate2(rAngle);
    
    x=cat.interpolateUsingFixedAngle((key+g_angle/360.0f))[0];
    y=cat.interpolateUsingFixedAngle((key+g_angle/360.0f))[1];
    z=cat.interpolateUsingFixedAngle((key+g_angle/360.0f))[2];
    
    float dx=x-px;
    float dz=z-pz;
    
    float angle=-atanf(dz/dx)+PI/2;
    cout << angle*(180.0f/PI)<< endl;
    
    px=x;
    py=y;
    pz=z;

    torso->translate(x, y, z);
    
    torso->selfRotate(0,angle,0,true);
    
    // Draw the models
    torso->draw(0,true);
    
    
    //glutSolidTeapot(0.5f);
}

void render( void ) {
    renderReady();
    if(mode==1){
        HW1(g_angle,key,maxKey);
    }
    else if(mode==2){
        HW2();
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