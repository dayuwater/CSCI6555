//
//  HW2.cpp
//  CSCI6555
//
//  Created by Tan Wang on 10/27/15.
//  Copyright © 2015 Tan Wang. All rights reserved.
//

#include <stdio.h>
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
#include "Models.h"
#include "Util.h"

#define RESOURCE_DIR   "/Users/tanwang/Documents/CSCI6555/CSCI6555/resources/"

using namespace std;
using namespace Util::Constants;

void HW2(float g_angle,int key,int maxKey,float x,float y,float z,float px,float py,float pz){
    
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

