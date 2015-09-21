//
//  Splines.h
//  CSCI6555
//
//  Created by Tan Wang on 9/21/15.
//  Copyright © 2015 Tan Wang. All rights reserved.
//

#ifndef Splines_h
#define Splines_h
#include "Matrix.h"


namespace Animations {
    
    class Spline{
    public:
        // if a general spline is created, the matrix is a 4*4 identity matrix by default
        Spline(){
            _m=Matrix();
        }
        
        Matrix getMatrix(){
            return _m;
        }
        // for future use
        void setMatrix(Matrix s){
            _m=s;
        }
        
        // insert positions with Euler angles
        void addPosWithAngle(float x, float y,float z,float rx, float ry,float rz){
            _x.push_back(x);
            _y.push_back(y);
            _z.push_back(z);
            _rx.push_back(rx);
            _ry.push_back(ry);
            _rz.push_back(rz);
            
            
        }
        
        // insert positions with quaternions
        void addPosWithQuaternion(float x, float y, float z,float qw, float qa,float qb,float qc){
            _x.push_back(x);
            _y.push_back(y);
            _z.push_back(z);
            _qw.push_back(qw);
            _qa.push_back(qa);
            _qb.push_back(qb);
            _qc.push_back(qc);
            
        }
    protected:
        
        // the position of key frames
        vector<float> _x;
        vector<float> _y;
        vector<float> _z;
        
        // the fixed/Euler angle of key frames
        vector<float> _rx;
        vector<float> _ry;
        vector<float> _rz;
        
        // the quaternion of key frames
        vector<float> _qw;
        vector<float> _qa;
        vector<float> _qb;
        vector<float> _qc;
        
        
    private:
        Matrix _m;
        
        
        
    };
    
    class CatmulRomSpline : public Spline
    {
    public:
        CatmulRomSpline(){
            float matNums[]={-0.5f,1.5f,-1.5f,0.5f,1.0f,-2.5f,2.0f,-0.5f,-0.5f,0.0f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f};
            _CRMatrix=Matrix(matNums,4,4);
        }
        
        Matrix getMatrix(){
            return _CRMatrix;
        }
        
        void setMatrix(){
            std::cout << "You cannot set the matrix" << endl;
        }

    private:
        Matrix _CRMatrix;
        
        
    };
    
    class BSpline: public Spline
    {
    public:
        BSpline(){
            float matNums[]={-1,3,-3,1,3,-6,3,0,-3,0,3,0,1,4,1,0};
            _BMatrix=Matrix(matNums,4,4);

        }
        Matrix getMatrix(){
            return _BMatrix;
        }
        
        void setMatrix(){
            std::cout << "You cannot set the matrix" << endl;
        }
    private:
        // Remember to divide by 6 when doing animation
        Matrix _BMatrix;
        
    };
    
    
}



#endif /* Splines_h */
