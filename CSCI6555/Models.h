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
    Model(float x=0,float y=0,float z=0){
        _x=x;
        _y=y;
        _z=z;
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
    
    Model getParent(){
        return parent[0];
    }
    
    
    // position of the model
    float _x;
    float _y;
    float _z;
protected:
    vector<Model> parent;
    vector<Model> children;
   
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
