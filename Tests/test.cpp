//
//  main.cpp
//  Tests
//
//  Unit Test for my Quaternion Class using Google Test
//  Created by Tan Wang on 9/3/15.
//  Copyright (c) 2015 Tan Wang. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"
#include "TestClass.h"
#include "Quaternion.h"
#include "math.h"
#include "Splines.h"
#define PI 3.14159265358979




Quaternion q; // Empty Quaternion

// My test cases
Quaternion qu1(PI/6,1,0,0,true);
Quaternion qu2(PI/4,0,1,0,false);
Quaternion qu3(PI/3,0,0,1,false);

// Test cases in slides
Quaternion qu4(PI,1,0,0,true);
Quaternion qu5(PI,0,1,0,true);
Quaternion qu6(PI,0,0,1,true);


// Part 1: Core Test

TEST(Creation,Empty){
    EXPECT_EQ(0, q._w);
    EXPECT_EQ(0, q._a);
    EXPECT_EQ(0, q._b);
    EXPECT_EQ(0, q._c);
}

TEST(Creation,Unit){
    
    // My examples
    /*EXPECT_FLOAT_EQ(0.43301270, qu1._w);
    EXPECT_FLOAT_EQ(0.25, qu1._a);
    EXPECT_FLOAT_EQ(0, qu1._b);
    EXPECT_FLOAT_EQ(0, qu1._c);
    
    EXPECT_FLOAT_EQ(0.35355339059, qu2._w);
    EXPECT_FLOAT_EQ(0, qu2._a);
    EXPECT_FLOAT_EQ(0.35355339059, qu2._b);
    EXPECT_FLOAT_EQ(0, qu2._c);*/
    
    
    // Example from slides
    EXPECT_NEAR(0, qu4._w,0.00001);
    EXPECT_NEAR(1, qu4._a,0.00001);
    EXPECT_NEAR(0, qu4._b,0.00001);
    EXPECT_NEAR(0, qu4._c,0.00001);
    
    EXPECT_NEAR(0, qu5._w,0.00001);
    EXPECT_NEAR(0, qu5._a,0.00001);
    EXPECT_NEAR(1, qu5._b,0.00001);
    EXPECT_NEAR(0, qu5._c,0.00001);
    
    EXPECT_NEAR(0, qu6._w,0.00001);
    EXPECT_NEAR(0, qu6._a,0.00001);
    EXPECT_NEAR(0, qu6._b,0.00001);
    EXPECT_NEAR(1, qu6._c,0.00001);

    
    
}

TEST(Operations,Square){
    EXPECT_FLOAT_EQ(qu4.square(), 1.00000);
    EXPECT_FLOAT_EQ(qu5.square(), 1.00000);

    EXPECT_FLOAT_EQ(qu1.square(), 1.00000);
    Quaternion j= Quaternion(2,1,3,5,true);
    EXPECT_FLOAT_EQ(j.square(), 1.000000);
    Quaternion k= Quaternion(2.9,7,23,87,true);
    EXPECT_FLOAT_EQ(k.square(), 1.000000);


}

TEST(Operations,Multiply){
    Quaternion a(0,0,1,0,false);
    Quaternion b(0,0,0,1,false);
    
    EXPECT_NEAR(a.multiply(b)._w,0,0.0001);
    EXPECT_NEAR(a.multiply(b)._a,1,0.0001);
    EXPECT_NEAR(a.multiply(b)._b,0,0.0001);
    EXPECT_NEAR(a.multiply(b)._c,0,0.0001);
}

TEST(Opeations,Inverse){
    Quaternion a=qu4.inverse();
    Quaternion b=qu5.inverse();
    Quaternion c=qu6.inverse();
    
    EXPECT_NEAR(qu4.multiply(qu5).multiply(b)._w,qu4._w , 0.0001);
    EXPECT_NEAR(qu4.multiply(qu5).multiply(b)._a,qu4._a , 0.0001);
    EXPECT_NEAR(qu4.multiply(qu5).multiply(b)._b,qu4._b , 0.0001);
    EXPECT_NEAR(qu4.multiply(qu5).multiply(b)._c,qu4._c , 0.0001);
    
    EXPECT_NEAR(qu5.multiply(qu6).multiply(c)._w,qu5._w , 0.0001);
    EXPECT_NEAR(qu5.multiply(qu6).multiply(c)._a,qu5._a , 0.0001);
    EXPECT_NEAR(qu5.multiply(qu6).multiply(c)._b,qu5._b , 0.0001);
    EXPECT_NEAR(qu5.multiply(qu6).multiply(c)._c,qu5._c , 0.0001);
    
    EXPECT_NEAR(qu1.multiply(qu2).multiply(qu2.inverse())._w,qu1._w , 0.0001);
    EXPECT_NEAR(qu1.multiply(qu2).multiply(qu2.inverse())._a,qu1._a , 0.0001);
    EXPECT_NEAR(qu1.multiply(qu2).multiply(qu2.inverse())._b,qu1._b , 0.0001);
    EXPECT_NEAR(qu1.multiply(qu2).multiply(qu2.inverse())._c,qu1._c , 0.0001);
    
    Quaternion r=qu2.multiply(qu2.inverse());  // The identity of a quaternion is (1,0,0,0)
    
    

    
}

TEST(Operations, Add){
    Quaternion a(1,1,1,1,false);
    Quaternion b(2,3,4,5,false);
    EXPECT_NEAR((a+b)._w, 3, 0.00001);
    EXPECT_NEAR((a+b)._a, 4, 0.00001);
    EXPECT_NEAR((a+b)._b, 5, 0.00001);
    EXPECT_NEAR((a+b)._c, 6, 0.00001);
    
}

TEST(Transformation, FixedAngleToQuatenion){
    Quaternion q=Quaternion::fixedAngle(0,PI,PI);
    
    EXPECT_NEAR(q._w, 0, 0.0001);
    EXPECT_NEAR(q._a, 1, 0.0001);
    EXPECT_NEAR(q._b, 0, 0.0001);
    EXPECT_NEAR(q._c, 0, 0.0001);
    
}

TEST(Transformation,Matrix){
    Quaternion q=Quaternion::fixedAngle(0, 0, PI);
    for(int i=0; i<16; i++){
        cout << q.rMatrix()[i];
        if(i%4==3){
            cout << endl;
        }
    }
    //assert(false);
    
}


// Part 2: Animation Test
TEST(Spline, Create){
    Animations::Spline s;
    //assert(false);
    
}

TEST(CRS,Create){
    Animations::CatmulRomSpline s;
    //assert(false);

}

TEST(BS,Create){
    Animations::BSpline s;
    //assert(false);
}


int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
