//
//  TestClass.h
//  CSCI6555
//
//  Created by Tan Wang on 9/3/15.
//  Copyright (c) 2015 Tan Wang. All rights reserved.
//

#ifndef CSCI6555_TestClass_h
#define CSCI6555_TestClass_h


class TestClass {
public:
    int addition(int a, int b)
    {
        return a + b;
    }
    
    int subtraction(int a,int b){
        return a-b;
    }
    int mult(int a,int b){
        return a*b;
    }
    int div(int a,int b){
        return a/b;
    }
    int pow(int a,int b){
        int r=a;
        for(int i=0; i<b-1; i++){
            r*=b;
        }
        return r;
    }
    

};


#endif
