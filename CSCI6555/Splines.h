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
        
        // interpolate using fixed angle
        vector<float> interpolateUsingFixedAngle(float time){
            vector<float> result;
            
            result.resize(6);
            // integer part of the time
            int intPart=(int)time;
            // decimal part of the time
            float decPart=time-intPart;
            
           
            
            
            
            float trans[]={(float)pow(decPart,3),(float)pow(decPart,2),decPart,1};
            Matrix T=Matrix(trans,1,4);
            
            float Xpoints[]={_x[intPart],_x[intPart+1],_x[intPart+2],_x[intPart+3]};
            Matrix xG=Matrix(Xpoints,4,1);
            
            float Ypoints[]={_y[intPart],_y[intPart+1],_y[intPart+2],_y[intPart+3]};
            Matrix yG=Matrix(Ypoints,4,1);
            
            float Zpoints[]={_z[intPart],_z[intPart+1],_z[intPart+2],_z[intPart+3]};
            Matrix zG=Matrix(Zpoints,4,1);
            
            float XRpoints[]={_rx[intPart],_rx[intPart+1],_rx[intPart+2],_rx[intPart+3]};
            Matrix xrG=Matrix(XRpoints,4,1);
            
            float YRpoints[]={_ry[intPart],_ry[intPart+1],_ry[intPart+2],_ry[intPart+3]};
            Matrix yrG=Matrix(YRpoints,4,1);
            
            float ZRpoints[]={_rz[intPart],_rz[intPart+1],_rz[intPart+2],_rz[intPart+3]};
            Matrix zrG=Matrix(ZRpoints,4,1);
            
            result[0]=T.multiply(_m).multiply(xG).get(0, 0);
            result[1]=T.multiply(_m).multiply(yG).get(0, 0);
            result[2]=T.multiply(_m).multiply(zG).get(0, 0);
            result[3]=T.multiply(_m).multiply(xrG).get(0, 0);
            result[4]=T.multiply(_m).multiply(yrG).get(0, 0);
            result[5]=T.multiply(_m).multiply(zrG).get(0, 0);
            
            
            
            //result: x->y->z->rx->ry->rz
            return result;
        }
        
        
        // interpolate using quaternion
        vector<float> interpolateUsingQuaternion(float time){
            vector<float> result;
            result.resize(7);
            
            // integer part of the time
            int intPart=(int)time;
            // decimal part of the time
            float decPart=time-intPart;
            
            
            
            
            
            float trans[]={(float)pow(decPart,3),(float)pow(decPart,2),decPart,1};
            Matrix T=Matrix(trans,1,4);
            
            float Xpoints[]={_x[intPart],_x[intPart+1],_x[intPart+2],_x[intPart+3]};
            Matrix xG=Matrix(Xpoints,4,1);
            
            float Ypoints[]={_y[intPart],_y[intPart+1],_y[intPart+2],_y[intPart+3]};
            Matrix yG=Matrix(Ypoints,4,1);
            
            float Zpoints[]={_z[intPart],_z[intPart+1],_z[intPart+2],_z[intPart+3]};
            Matrix zG=Matrix(Zpoints,4,1);
            
            float QWpoints[]={_qw[intPart],_qw[intPart+1],_qw[intPart+2],_qw[intPart+3]};
            Matrix qwG=Matrix(QWpoints,4,1);
            
            
            float QApoints[]={_qa[intPart],_qa[intPart+1],_qa[intPart+2],_qa[intPart+3]};
            Matrix qaG=Matrix(QApoints,4,1);
            
            float QBpoints[]={_qb[intPart],_qb[intPart+1],_qb[intPart+2],_qb[intPart+3]};
            Matrix qbG=Matrix(QBpoints,4,1);
            
            float QCpoints[]={_qc[intPart],_qc[intPart+1],_qc[intPart+2],_qc[intPart+3]};
            Matrix qcG=Matrix(QCpoints,4,1);

            
            result[0]=T.multiply(_m).multiply(xG).get(0, 0);
            result[1]=T.multiply(_m).multiply(yG).get(0, 0);
            result[2]=T.multiply(_m).multiply(zG).get(0, 0);
            result[3]=T.multiply(_m).multiply(qwG).get(0, 0);
            result[4]=T.multiply(_m).multiply(qaG).get(0, 0);
            result[5]=T.multiply(_m).multiply(qbG).get(0, 0);
            result[6]=T.multiply(_m).multiply(qcG).get(0,0);
            

            
            //result: x->y->z->qw->qa->qb->qc
            return result;
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
        
        
    
        Matrix _m;
        
        
        
    };
    
    class CatmulRomSpline : public Spline
    {
    public:
        CatmulRomSpline(){
            float matNums[]={-0.5f,1.5f,-1.5f,0.5f,1.0f,-2.5f,2.0f,-0.5f,-0.5f,0.0f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f};
            _m=Matrix(matNums,4,4);
            
        }
        
        
        
        void setMatrix(){
            std::cout << "You cannot set the matrix" << endl;
        }

        
        
    };
    
    class BSpline: public Spline
    {
    public:
        BSpline(){
            float matNums[]={-1.0f,3.0f,-3.0f,1.0f,3.0f,-6.0f,3.0f,0.0f,-3.0f,0.0f,3.0f,0.0f,1.0f,4.0f,1.0f,0.0f};
            _m=Matrix(matNums,4,4);
            _m=_m.scale(6.0f, 0x94);

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
