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
#include "Vector.h"

using namespace std;

namespace Util {
    
    namespace Constants{
        Vec POSITION(0.0f,-0.5f,-5.0f);
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
}


#endif /* Util_h */
