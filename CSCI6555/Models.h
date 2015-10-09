//
//  Header.h
//  CSCI6555
//
//  Created by Tan Wang on 10/8/15.
//  Copyright © 2015 Tan Wang. All rights reserved.
//

#ifndef Models_h
#define Models_h

#include "Quaternion.h"
#include "Splines.h"
#include "Matrix.h"

// Hierachical Model class
class Model{
public:
    
    // create a model and put it into a specific location
    Model(float x=0,float y=0,float z=0,float rx=0, float ry=0,float rz=0,float qw=0,float qa=0,float qb=0,float qc=0 ){
        _x=x;
        _y=y;
        _z=z;
        _rx=rx;
        _ry=ry;
        _rz=rz;
        _qw=qw;
        _qa=qa;
        _qb=qb;
        _qc=qc;
        children.resize(0);
    }
   
    // use OpenGL function here
    void draw(){
        
        
    }
    void addChild(Model &m){
        children.push_back(m);
        m.parent.push_back(*this);
    }
    // rotate about the parent
    void rotate(float angle){
        
    }
    // self rotate, mode=0 is fixed angle, mode=1 is quaternion
    void selfRotate(int mode){
        if(mode==0){
            Quaternion r=Quaternion::fixedAngle(_rx,_ry , _rz);
            glMultMatrixf(r.rMatrix());
        }
        else if(mode==1){
            Quaternion r(_qw,_qa , _qb,_qc);
            glMultMatrixf(r.rMatrix());
            
        }
        
    }
    
    Model getParent(){
        return parent[0];
    }
    
    
    // position of the model
    float _x;
    float _y;
    float _z;
    // orientation using fixed angle
    float _rx;
    float _ry;
    float _rz;
    // orientation using Quaternion
    float _qw;
    float _qa;
    float _qb;
    float _qc;
protected:
    vector<Model> parent;
    vector<Model> children;
   
   
    
};

class Cube : public Model{
public:
    Cube(float sizee=0.5f,float x=0.0f,float y=0.0f,float z=-5.0f){
        size=sizee;
        _x=x;
        _y=y;
        _z=z;
    }
    float size;
    
    void draw(int mode=0){
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        selfRotate(mode);
        glutSolidCube(size);
    }
};



#endif /* Header_h */
