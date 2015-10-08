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
    Model(){
        children.resize(0);
    }
    // create a model and put it into a specific location
    Model(float x,float y,float z){
        _x=x;
        _y=y;
        _z=z;
    }
   
    // use OpenGL function here
    void draw(){
        
        
    }
    void addChild(Model m){
        children.push_back(m);
        m.parent()=*this;
    }
    // rotate about the parent
    void rotate(float angle){
        
    }
protected:
    Model parent();
    vector<Model> children;
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
    
};


#endif /* Header_h */
