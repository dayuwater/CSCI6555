//
//  Quaternion.h
//  CSCI6555
//
//  Created by Tan Wang on 9/11/15.
//  Copyright (c) 2015 Tan Wang. All rights reserved.
//

#ifndef CSCI6555_Quaternion_h
#define CSCI6555_Quaternion_h

#include <OpenGL/OpenGL.h>
#include <vector>
#include "Matrix.h"
#include "math.h"

using namespace std;

class Quaternion {
public:
    
    // Default constructor (Done)
    Quaternion(){
        _w=0;
        _a=0;
        _b=0;
        _c=0;
    }
       // Create a quaternion directly.
    // rotation=true means transfering the parameters to a unit rotation quaternion
    // rotation=false means  keeping the parameters
    Quaternion(float w,float a,float b,float c,bool rotation=true){
        if(rotation){
            _w=cosf(w/2);
            float vecLength=sqrtf(a*a+b*b+c*c);
            _a=a/vecLength*sinf(w/2);
            _b=b/vecLength*sinf(w/2);
            _c=c/vecLength*sinf(w/2);
        }
        else{
            _w=w;
            _a=a;
            _b=b;
            _c=c;
        }
    }
    
    // Create a quaternion using a Fixed or Eular angle
    // The direction of rotation is x->y->z
    static Quaternion fixedAngle(float x,float y, float z){
        Quaternion a(x,1,0,0,true);
        Quaternion b(y,0,1,0,true);
        Quaternion c(z,0,0,1,true);
        Quaternion r=a.multiply(b).multiply(c);
        return r;
        
    }

    // Normalize a quaternion
    Quaternion normalize(){
        Quaternion q;
        return q;
    }
    
    //get the rotation matrix (Depricated)
    vector<GLfloat> rotationMatrix(){
        vector<GLfloat> result;
        
        
        return result;
    }
    Matrix rotMatrix(){
        float result[16];
        result[0]=1-2*_b*_b-2*_c*_c;
        result[1]=2*_a*_b-2*_w*_c;
        result[2]=2*_a*_c+2*_w*_b;
        result[3]=0;
        
        result[5]=1-2*_a*_a-2*_c*_c;
        result[4]=2*_a*_b+2*_w*_c;
        result[6]=2*_b*_c-2*_w*_a;
        result[7]=0;
        
        result[10]=1-2*_a*_a-2*_b*_b;
        result[8]=2*_a*_c-2*_w*_b;
        result[9]=2*_b*_c+2*_w*_a;
        result[11]=0;
        
        result[12]=0;
        result[13]=0;
        result[14]=0;
        result[15]=1;

        Matrix m(result,4,4);
        return m;
    }
    
    
    
    GLfloat* rMatrix(){
        GLfloat* result=(GLfloat*)malloc(16*sizeof(GLfloat));
        result[0]=1-2*_b*_b-2*_c*_c;
        result[1]=2*_a*_b-2*_w*_c;
        result[2]=2*_a*_c+2*_w*_b;
        result[3]=0;
        
        result[5]=1-2*_a*_a-2*_c*_c;
        result[4]=2*_a*_b+2*_w*_c;
        result[6]=2*_b*_c-2*_w*_a;
        result[7]=0;
        
        result[10]=1-2*_a*_a-2*_b*_b;
        result[8]=2*_a*_c-2*_w*_b;
        result[9]=2*_b*_c+2*_w*_a;
        result[11]=0;
        
        result[12]=0;
        result[13]=0;
        result[14]=0;
        result[15]=1;
        return result;
    }
    
    // quaternion multiplication
    Quaternion multiply(Quaternion q){
        Quaternion r;
        r._w=_w*q._w-(_a*q._a+_b*q._b+_c*q._c);
        r._a=_w*q._a+_a*q._w+_b*q._c-_c*q._b;
        r._b=_w*q._b+_b*q._w+_c*q._a-_a*q._c;
        r._c=_w*q._c+_c*q._w+_a*q._b-_b*q._a;
        return r;
    }
    
    // |q|^2 (Done)
    float square(){
        return _w*_w+_a*_a+_b*_b+_c*_c;
    }
    
    float dot(Quaternion b){
        return _w*b._w+_a*b._a+_b*b._b+_c*b._c;
    }
    
    // inverse
    Quaternion inverse(){
        Quaternion q;
        q._w=_w/square();
        q._a=-_a/square();
        q._b=-_b/square();
        q._c=-_c/square();
        
        return q;
    }
    
    // basic operations
    Quaternion operator+ (Quaternion b){
        Quaternion r;
        r._w=_w+b._w;
        r._a=_a+b._a;
        r._b=_b+b._b;
        r._c=_c+b._c;
        return r;
    }
    Quaternion operator- (Quaternion b){
        Quaternion r;
        r._w=_w-b._w;
        r._a=_a-b._a;
        r._b=_b-b._b;
        r._c=_c-b._c;
        return r;
    }
    Quaternion operator* (float b){
        Quaternion r;
        r._w=_w*b;
        r._a=_a*b;
        r._b=_b*b;
        r._c=_c*b;
        return r;
    }

    Quaternion operator/ (float b){
        Quaternion r;
        r._w=_w/b;
        r._a=_a/b;
        r._b=_b/b;
        r._c=_c/b;
        return r;
    }


    
    
    //slerp
    Quaternion slerp(Quaternion q,float u){
        float omega=acosf(dot(q));
        Quaternion r;
        float fac1=(sinf(1-u)*omega)/sinf(omega);
        float fac2=sinf(omega*u)/sinf(omega);
        r=(*this)*fac1+q*fac2;
        return r;
    }
    
    
    
    // print the quaternion
    string toString(){
        return "";
        
    }
    
    
    
    
    
    
    // "Private" fields: A Quaternion represented by w+ai+bj+ck
    float _w; // The scalar part
    float _a; // i part
    float _b; // j part
    float _c; // k part
    
};


#endif
