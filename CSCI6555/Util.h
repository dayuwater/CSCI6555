//
//  Util.h
//  CSCI6555
//
//  Created by Tan Wang on 10/10/15.
//  Copyright © 2015 Tan Wang. All rights reserved.
//

#ifndef Util_h
#define Util_h
#include <math.h>
#include <assert.h>
#include "Vector.h"

using namespace std;

namespace Util {
    
    namespace Constants{
        Vec POSITION(0.0f,-0.5f,-5.0f);
        float PI=3.14159265358979;
    }
    
    vector<float> solveX2(float a, float b, float c){
        vector<float> result;
        result.resize(2);
        float delta=b*b-4*a*c;
        if(delta<0){
            cout << "This equation has no real solution" << endl;
            result[0]=0;
            result[1]=0;
            
        }
        else{
            result[0]=(-b+sqrt(delta))/2/a;
            result[1]=(-b-sqrt(delta))/2/a;
        }
        
        return result;
        
    }
    
    float rotateFunction(float angle,int mode=0){
        //assert((Constants::PI/2)*sinf(angle)==0);
        if(mode==0){
            return Constants::PI+(Constants::PI/4)*sinf(angle);
        }
        else{
            return Constants::PI-(Constants::PI/4)*sinf(angle);

        }
    }
}


#endif /* Util_h */
