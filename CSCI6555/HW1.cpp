//
//  HW1.cpp
//  CSCI6555
//
//  Created by Tan Wang on 10/8/15.
//  Copyright © 2015 Tan Wang. All rights reserved.
//


#include <iostream>
#define PI 3.14159265358979

using namespace std;

void HW1(float g_angle,int key,int maxKey){
    // clear buffer
    
    
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
            frameX[i][j]=0.0f+(0.5f+i*0.5f)*cosf(PI/3*j);
            frameY[i][j]=-0.0f+(0.5f+i*0.5f)*sinf(PI/3*j);
            frameZ[i][j]=-5.0f;
            frameQW[i][j]=g_angle/(180.0f/PI);
            if(j%3==0){
                frameXR[i][j]=g_angle/(180.0f/PI);
                frameYR[i][j]=0;
                frameZR[i][j]=0;
                frameQA[i][j]=1.0f;
                frameQB[i][j]=0;
                frameQC[i][j]=0;
            }
            if(j%3==1){
                frameXR[i][j]=0;
                frameYR[i][j]=g_angle/(180.0f/PI);
                frameZR[i][j]=0;
                frameQA[i][j]=0;
                frameQB[i][j]=1.0f;
                frameQC[i][j]=0;
            }
            
            if(j%3==2){
                frameXR[i][j]=0;
                frameYR[i][j]=0;
                frameZR[i][j]=g_angle/(180.0f/PI);
                frameQA[i][j]=0;
                frameQB[i][j]=0;
                frameQC[i][j]=1.0f;
            }
            
            
        }
    }
    
    // store the positions into spline instances
    Animations::CatmulRomSpline crs[4];
    Animations::BSpline bs[4];
    
    for(int i=0; i<4; i++){
        for(int j=0; j<7; j++){
            // Fixed angle objects
            if(i<2){
                crs[i].addPosWithAngle(frameX[i][j], frameY[i][j], frameZ[i][j], frameXR[i][j]
                                       , frameYR[i][j], frameZR[i][j]);
                bs[i].addPosWithAngle(frameX[i][j], frameY[i][j], frameZ[i][j], frameXR[i][j]
                                      , frameYR[i][j], frameZR[i][j]);
                
                
            }
            // Quaternion objects
            else{
                crs[i].addPosWithQuaternion(frameX[i][j], frameY[i][j], frameZ[i][j], frameQW[i][j]
                                            , frameQA[i][j], frameQB[i][j],frameQC[i][j]);
                bs[i].addPosWithQuaternion(frameX[i][j], frameY[i][j], frameZ[i][j], frameQW[i][j]
                                           , frameQA[i][j], frameQB[i][j],frameQC[i][j]);
                
                
                
                
            }
        }
    }
    
    glLoadIdentity();
    glColor3f(1,0,0);
    
    for(int i=0; i<4; i++){
        glBegin(GL_LINES);
        for(int j=0; j<maxKey; j++){
            glVertex3f(frameX[i][j],frameY[i][j],frameZ[i][j]);
        }
        glEnd();
        
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
    
    //
    // i=0, the bottom one, use Catmul-Rom spline with Euler Angles
    // i=1, one above i=0,use B-spline with Euler Angles
    // i=2, one below i=3, use Catmul-Rom spline with Quaternion
    // i=3, the top one, use B-spline with Quaternion
    // The rotation is about x first, then y, then z, and again, in all objects
    //
    for(int i=0; i<4; i++){
        if(i<2){
            vector<float> currentPos;
            if(i==0){
                currentPos=crs[i].interpolateUsingFixedAngle(key+g_angle/360.0f);
            }
            else{
                currentPos=bs[i].interpolateUsingFixedAngle(key+g_angle/360.0f);
            }
            
            currentX[i]=currentPos[0];
            currentY[i]=currentPos[1];
            
            currentZ[i]=currentPos[2];
            currentXR[i]=currentPos[3];
            currentYR[i]=currentPos[4];
            currentZR[i]=currentPos[5];
        }
        else{
            vector<float> currentPos;
            if(i==2){
                currentPos=crs[i].interpolateUsingQuaternion(key+g_angle/360.0f);
            }
            else{
                currentPos=bs[i].interpolateUsingQuaternion(key+g_angle/360.0f);
            }
            currentX[i]=currentPos[0];
            currentY[i]=currentPos[1];
            
            currentZ[i]=currentPos[2];
            
            currentQW[i]=currentPos[3];
            currentQA[i]=currentPos[4];
            currentQB[i]=currentPos[5];
            currentQC[i]=currentPos[6];
            
            
            
        }
        
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
            Quaternion t(g_angle/59.0f,1,1,0,true);
            r=s.rMatrix();
        }
        
        
        
        glMultMatrixf(r);//
        //glRotated(g_angle, 1.0, 1.0, 1.0);
        
        
        
        
        // render objects
        
        glutSolidTeapot(0.125f+0.0000f*(i+1));
        
        
        
    }
    
    
    // disable lighting
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    
    // swap back and front buffers
    
}
