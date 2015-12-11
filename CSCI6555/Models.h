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
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <random>
#include <shared_mutex>
#include "Vector.h"
#include "Util.h"

#define PI 3.14159265358979




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
        children.push_back(&m);
        m.parent.push_back(this);
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
    
    Model* getParent(){
        return parent[0];
    }
    
    
    
    
    
    
    vector<Model*> getDecendents(){
        vector<Model*> result;
        
        if(children.size()==0){
            //result.push_back(*this);
            
        }
        else{
            for(int i=0; i<children.size();i++){
                vector<Model*> d=children[i]->getDecendents();
                
                for(int j=0; j<d.size();j++){
                    result.push_back(d[j]);
                }
                result.push_back(children[i]);
                
            }
        }
        
        
        
        return result;
    }
    
    
    // physics functions
    // ------------------------------
    //  Part 1: getters and setters
    // ------------------------------
    float getMass(){
        return _mass;
    }
    void setMass(float mass){
        if(mass<=0){
            return;
        }
        else{
            _mass=mass;
        }
    }
    
    // the name are in pure physics concepts
    // velocity=speed vector, speed=magnitude
    Vec getVelocity(){
        return _speed;
    }
    float getSpeed(){
        return _speed.length();
    }
    void setVelocity(Vec v){
        _speed=v;
    }
    
    
    Vec getAcceleration(){
        return _acc;
    }
    float getAccMagitude(){
        return _acc.length();
    }
    void setAcceleration(Vec a){
        _acc=a;
    }
    
    void setSpin(float x, float y, float z){
        _omegax=x;
        _omegay=y;
        _omegaz=z;
    }
    
    // ------------------------------
    //  Part 2: Force System
    // ------------------------------
    
    void addForce(Quaternion f){
        _forces.push_back(f);
    }
    void addForce(Vec v){
        
    }
    // get the sum of forces in x, y and z direction
    Vec getAxisForces(){
        Vec result;
        float x=0;
        float y=0;
        float z=0;
        for(int i=0; i<_forces.size();i++){
            // the vertical angle=tan-1(z/(x^2+y^2)^(1/2))
            float theta=atanf((_forces[i]._c)/sqrtf(_forces[i]._a*_forces[i]._a+_forces[i]._b*_forces[i]._b));
            // the horizontal angle=tan-1(y/x)
            if(isnan(theta)){
                theta=PI/2;
            }
            float phi=atanf(_forces[i]._b/_forces[i]._a);
            if((isnan(phi))){
                
                phi=PI/2;
                
            }
            x+=_forces[i]._w*cosf(theta)*cosf(phi);
            y+=_forces[i]._w*cosf(theta)*sinf(phi);
            z+=_forces[i]._w*sinf(theta);
        }
        result.set(x,y,z);
        return result;
    }
    // set the acceleration according to force
    void setAccByForce(){
        Vec f=getAxisForces();
        _acc=f/_mass; // -F-> = m -a->
    }
    
    // ------------------------------
    //  Part 3: Numerical Integration
    // ------------------------------
    
    
    void refresh(float dt=1.0f){
        setNewVelocity(dt);
        setNewPosition(dt);
        setNewAngle(dt);
        if(checkCollide()){
            Vec newSpeed;
            newSpeed.set(_speed.x(),_speed.y()*0.9f,_speed.z()); // if collide, inverse the motion, the -0.9 is the estimation of energy loss
            //_speed=newSpeed;
            
            _speed=_speed*(-1.0f);
            // a very simple approximation
            _omegax=-_omegax;
            _omegay=-_omegay;
            _omegaz=-_omegaz;
            
            
            setNewPosition(dt);
            setNewAngle(dt);
            
        }
        /*else if(checkCollideCube()){
         Vec newSpeed;
         newSpeed.set(_speed.x(),_speed.y()*(-0.9),_speed.z()); // if collide, inverse the motion, the -0.9 is the estimation of energy loss
         _speed=newSpeed;
         }*/
    }
    
    // "integrate" acceleration to get velocity
    void setNewVelocity(float dt=1.0f){
        _speed=_speed+_acc*dt;
    }
    
    // "integrate" velocity to get position
    void setNewPosition(float dt=1.0f){
        
        _x=_x+_speed.x()*dt;
        _y=_y+_speed.y()*dt;
        _z=_z+_speed.z()*dt;
    }
    
    void setNewAngle(float dt=1.0f){
        _rx=_rx+_omegax*dt;
        _ry=_ry+_omegay*dt;
        _rz=_rz+_omegaz*dt;
        
    }
    
    
    
    
    
    
    
    
    // position of the model
    float _x;
    float _y;
    float _z;
    // orientation using fixed angle
    float _rx;
    float _ry;
    float _rz;
    // angular velocity
    float _omegax;
    float _omegay;
    float _omegaz;
    // orientation using Quaternion
    float _qw;
    float _qa;
    float _qb;
    float _qc;
    
    
    float size;
    float _radius;
    string _type;
    string _sex;
protected:
    vector<Model*> parent;
    vector<Model*> children;
    
    
    
    
    // physical properties
    float _mass;
    Vec _speed;
    Vec _acc; // acceleration
    vector<Quaternion> _forces; // since force consist of magitude of the force and the direction of force
    // it is best to be represented by a quaternion, in this case rotational rules does not apply
    
    
    
    
    // collision detection using bounding sphere
    bool checkCollide(){
        vector<Model*> checkable=parent[0]->children;
        for(int i=0; i<checkable.size();i++){
            if(checkable[i]!=this){
                Vec FOI=Vec(checkable[i]->_x,checkable[i]->_y,checkable[i]->_z);
                Vec here=Vec(_x,_y,_z);
                Vec distance=here-FOI;
                float dis=distance.length();
                // if distance of the center is less than the sum of radius, there must be intersection
                if(dis<_radius+checkable[i]->_radius/2){
                    return true;
                }
            }
        }
        return false;
        
    }
    // collsion detection using bounding cube
    bool checkCollideCube(){
        vector<Model*> checkable=parent[0]->children;
        for(int i=0; i<checkable.size();i++){
            if(checkable[i]!=this){
                Vec min;
                float size=checkable[i]->size;
                min.set(checkable[i]->_x-size,checkable[i]->_y-size,checkable[i]->_z-size);
                Vec max;
                max.set(checkable[i]->_x+size,checkable[i]->_y+size,checkable[i]->_z+size);
                if(_x+this->size>=min.x()){
                    
                    
                    if(_x-this->size<=max.x()){
                        
                        
                        if(_y+this->size>=min.y()){
                            
                            if(_y-this->size<=max.y()){
                                
                                
                                
                                if(_z+this->size>=min.z()){
                                    
                                    
                                    
                                    if(_z-this->size<=max.z()){
                                        return true;
                                    }
                                    
                                    
                                    
                                }
                            }
                        }
                    }
                }
                
            }
        }
        return false;
    }
    
    
    
    
    
    
};

// this is not a good idea to use bounding sphere
class Cube : public Model{
public:
    Cube(float sizee=0.5f,float x=0.0f,float y=0.0f,float z=-5.0f){
        size=sizee;
        _x=x;
        _y=y;
        _z=z;
        
        _radius=size*sqrt(2);
    }
    
    
    void draw(int mode=0){
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        selfRotate(mode);
        glutSolidCube(size);
    }
};

// just to represent human in my project, the actual model is just a cone
class Human: public Model{
public:
    // constructor
    Human(float x=0.0f,float y=0.0f,float z=-5.0f){
        _x=x;
        _y=y;
        _z=z;
        setVelocity(Vec(-10+rand()%20,-10+rand()%20,-10+rand()%20)/120);
        
        
        
        
    }
    
    // factory methods for different kinds of human
    static Human* createMale1(float x, float y,float z){
        Human* m=new Human(x,y,z);
        //m->setVelocity(Vec(0.1f,0,0));
        m->setAcceleration(Vec(0,0,0));
        m->size=1.0f;
        m->_radius=0.5f;
        m->_sex="male";
        m->_type="HB";
        return m;
        
        
    }
    
    static Human* createMale2(float x, float y,float z){
        Human* m=new Human(x,y,z);
        //m->setVelocity(Vec(0.1f,0,0));
        m->setAcceleration(Vec(0,0,0));
        m->size=2.0f;
        m->_radius=0.5f;
        m->_sex="male";
        m->_type="HA";
        return m;
        
        
    }
    
    
    static Human* createFemale1(float x, float y, float z){
        Human* m=new Human(x,y,z);
        //m->setVelocity(Vec(0.1f,0,0));
        m->setAcceleration(Vec(0,0,0));
        m->size=1.0f;
        m->_radius=0.5f;
        m->_sex="female";
        m->_type="HB";
        return m;
    }
    
    static Human* createFemale2(float x, float y, float z){
        Human* m=new Human(x,y,z);
        //m->setVelocity(Vec(0.1f,0,0));
        m->setAcceleration(Vec(0,0,0));
        m->size=2.0f;
        m->_radius=0.5f;
        m->_sex="female";
        m->_type="HA";
        return m;
    }
    
    
    
    
    // refresh  function, if return =0 nothing happens
    // return from 1 to 3, there is a reproduction going on
    // 1, A-> A 2, A->B 3, B->B
    
    int refresh(float dt=1.0f){
        
        //assert(status!=3);
        setNewVelocity(dt);
        setNewPosition(dt);
        setNewAngle(dt);
        int status=checkReproduce();
        
        if(status>=0){
            Vec newSpeed;
            newSpeed.set(_speed.x(),_speed.y()*0.9f,_speed.z()); // if collide, inverse the motion, the -0.9 is the estimation of energy loss
            //_speed=newSpeed;
            
            _speed=_speed*(-1.0f);
            // a very simple approximation
            _omegax=-_omegax;
            _omegay=-_omegay;
            _omegaz=-_omegaz;
            
            
            setNewPosition(dt);
            setNewAngle(dt);
            //status=checkReproduce();
            
            
        }
        /*else if(checkCollideCube()){
         Vec newSpeed;
         newSpeed.set(_speed.x(),_speed.y()*(-0.9),_speed.z()); // if collide, inverse the motion, the -0.9 is the estimation of energy loss
         _speed=newSpeed;
         }*/
        return status;
    }
    
    // draw function
    void draw(int mode=0){
        glLoadIdentity();
        
        glTranslatef(_x, _y, _z);
        if(_sex=="female"){
            glRotatef(180, 1, 0, 0);
        }
        if(_sex=="male"){
            glRotatef(0, 1, 0, 0);
        }
        glScalef(size, size, size);
        
        glColor3f(1.0f, 0.0f, 0.0f);
        if(_type=="HA"){
            glutSolidCone(0.125, 1, 20, 20);
        }
        else{
            glutSolidTeapot(0.125);
        }
    }
    
    
protected:
    
    // behaviors
    // reproduction, also use to check collision
    // if return is not negative, there is a collision
    int checkReproduce(){
        vector<Model*> checkable=parent[0]->getDecendents();
        for(int i=0; i<checkable.size();i++){
            if(checkable[i]!=this){
                Vec FOI=Vec(checkable[i]->_x,checkable[i]->_y,checkable[i]->_z);
                Vec here=Vec(_x,_y,_z);
                Vec distance=here-FOI;
                float dis=distance.length();
                // if distance of the center is less than the sum of radius, there must be intersection
                if(dis<_radius+checkable[i]->_radius/2){
                    // if they are both human
                    if((_type=="HA"||_type=="HB")&&(checkable[i]->_type=="HA"||checkable[i]->_type=="HB")){
                        // if they have different sex
                        if(_sex!=checkable[i]->_sex){
                            if(_type=="HA"&&checkable[i]->_type=="HA"){
                                return 1;
                            }
                            else if(_type=="HB"&&checkable[i]->_type=="HB"){
                                return 3;
                            }
                            else{
                                return 2;
                            }
                        }
                    }
                    else{
                        return 0;
                    }
                    
                    
                }
            }
        }
        return -1;
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
};

// normal intelligent model
class IntelModel : public Model{
public:
    
    
    void refresh(float dt=1.0f,int leader=-1){
        // behavioral animation
        velocityArbitration(leader);
        // physical animation
        setNewVelocity(dt);
        setNewPosition(dt);
        setNewAngle(dt);
        if(checkCollide()){
            Vec newSpeed;
            newSpeed.set(_speed.x(),_speed.y()*0.9f,_speed.z()); // if collide, inverse the motion, the -0.9 is the estimation of energy loss
            //_speed=newSpeed;
            
            _speed=_speed*(-1.0f);
            // a very simple approximation
            _omegax=-_omegax;
            _omegay=-_omegay;
            _omegaz=-_omegaz;
            
            
            setNewPosition(dt);
            setNewAngle(dt);
            
        }
    }
    
    
    
private:
    
    // get velocity for flock centering
    Vec flockCenteringVelocity(float* factor){
        // get the center of other teapots
        vector<Model*> check=parent[0]->getDecendents();
        vector<Vec> checkablePos;
        for(int i=0; i<check.size();i++){
            // faked polymorphysm
            if(check[i]->_type=="intel"&&this!=check[i]){
                checkablePos.push_back(Vec(check[i]->_x,check[i]->_y,check[i]->_z));
            }
            
            
        }
        Vec center;
        float totalX=0;
        float totalY=0;
        float totalZ=0;
        for(int i=0; i<checkablePos.size();i++){
            totalX+=checkablePos[i].x();
            totalY+=checkablePos[i].y();
            totalZ+=checkablePos[i].z();
        }
        center.set(totalX/checkablePos.size(),totalY/checkablePos.size(),totalZ/checkablePos.size());
        
        
        // determine the velocity direction
        Vec dir;
        dir.set(-_x+center.x(),-_y+center.y(),-_z+center.z());
        
        // set new velocity and factor of centering
        // factor=distance-2 , 2 is the minumum distance
        *factor=dir.length()-1.5;
        return dir.normalize()*_speed.length();
        
        
    }
    // get velocity for velocity matching , leader is the index of leading teapot, if
    //leader=-1 the leader is randomly selected in each refresh
    Vec velocityMatchingVelocity(int leader=-1){
        // get the center of other teapots
        vector<Model*> check=parent[0]->getDecendents();
        vector<Vec> checkableVelocity;
        for(int i=0; i<check.size();i++){
            // faked polymorphysm
            if(check[i]->_type=="intel"&&this!=check[i]){
                checkableVelocity.push_back(Vec(check[i]->getVelocity().x(),check[i]->getVelocity().y(),check[i]->getVelocity().z()));
            }
            
            
        }
        Vec finalVelocity;
        if(leader==-1){
            
            finalVelocity=checkableVelocity[rand()%checkableVelocity.size()];
        }
        else{
            finalVelocity=checkableVelocity[leader];
        }
        
        
        
        
        // set new velocity
        return finalVelocity;
        
    }
    
    Vec collisonAvoidance(float* factor){
        Vec result;
        float fac=0;
        // get all real checkables
        vector<Model*> check=parent[0]->getDecendents(); // fetch
        vector<Model*> realCheckable; // 1st filter
        vector<Model*> closeModels; // 2nd filter
        vector<Model*> frontModels; // 3rd filter
        vector<float> angles;
        vector<float> distances;
        for(int i=0; i<check.size();i++){
            if(this!=check[i]){
                realCheckable.push_back(check[i]);
            }
        }
        
        // calculate the distance for all of them
        // check if they are closer than 1.5
        for(int i=0; i<realCheckable.size();i++){
            Vec centerA;
            centerA.set(realCheckable[i]->_x,realCheckable[i]->_y,realCheckable[i]->_z);
            Vec centerB;
            centerB.set(this->_x,this->_y,this->_z);
            if(centerA.distance(centerB)<realCheckable[i]->_radius+this->_radius+1.5){
                closeModels.push_back(realCheckable[i]);
            }
        }
        // find all models that appear at the front (-90 degree to 90 degree)
        for(int i=0; i<closeModels.size();i++){
            
            Vec B(getVelocity());
            Vec centerA;
            centerA.set(closeModels[i]->_x,closeModels[i]->_y,closeModels[i]->_z);
            Vec centerB;
            centerB.set(this->_x,this->_y,this->_z);
            Vec A(centerA-centerB);
            // a.b=ab(cos theta)
            float theta=0;
            float k=(A.dot(B))/(A.length()*B.length());
            theta=acosf(k);
            
            if(theta<PI/2){
                frontModels.push_back(closeModels[i]);
                angles.push_back(theta);
                distances.push_back(centerA.distance(centerB)-closeModels[i]->_radius-this->_radius);
                
            }
            
            
            
            
            
        }
        
        
        // turn to the closest safe angle
        sort(angles.begin(),angles.end());
        float maxAngle=0;
        if(!angles.empty()){
            maxAngle=angles[angles.size()-1];
        }
        // always stear right for now (x and y coordinates)
        result.set(getVelocity().x()*cos(maxAngle)-getVelocity().y()*sin(maxAngle),
                   getVelocity().x()*sin(maxAngle)+getVelocity().y()*cos(maxAngle),getVelocity().z());
        
        
        sort(distances.begin(),distances.end());
        if(!distances.empty()){
            *factor=1.5/((distances[0])*(distances[0])); // proportional to 1/(r^2)
        }
        else{
            *factor=0;
        }
        return result;
    }
    
    
    // collison avoidance
    
    // arbitration for three velocities
    void velocityArbitration(int leader=-1,float f1=0.0f, float f2=1.0f, float f3=2.0f,float f4=0.0f){
        Vec previousSpeed=_speed;
        float factor2=0;
        float factor4=0;
        Vec flockCentering=flockCenteringVelocity(&factor2);
        Vec velocityMatching=velocityMatchingVelocity(leader);
        Vec collisonAvoid=collisonAvoidance(&factor4);
        
        
        _speed=(previousSpeed*f1+flockCentering*factor2+velocityMatching*f3+collisonAvoid*factor4).normalize()*0.5;
        
    }
    
};


class Teapot : public IntelModel{
public:
    Teapot(float sizee=0.5f,float x=0.0f,float y=0.0f,float z=-5.0f,float ry=0){
        size=sizee;
        _x=x;
        _y=y;
        _z=z;
        
        _ry=ry;
        
        _radius=size;
        _type="intel";
    }
    
    
    void draw(int mode=0){
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        selfRotate(mode);
        glutSolidTeapot(size);
    }
};


// Model using the loaded data file
class PolyModel: public Model{
public:
    PolyModel (float x=0.0f,float y=0.0f,float z=-5.0f){
        for(int i=0; i<3; i++){
            maxVert.set(-9999999,-9999999,-9999999);
        }
        for(int i=0; i<3; i++){
            minVert.set(9999999,9999999,9999999);
        }
        _x=x;
        _y=y;
        _z=z;
        
    }
    
    void loadModel(istream& input){
        int vertex_count=0;
        int polys_count=0;
        string data;
        
        input >> data >> vertex_count >> polys_count;
        
        verts.resize(vertex_count);
        polys.resize(polys_count);
        poly_center.resize(polys_count);
        surround_polys.resize(vertex_count);
        
        for(int i=0; i<vertex_count; i++){
            Vec v;
            float a,b,c;
            input >> a >> b >> c;
            // find the maximum vertex
            (a>maxVert.x()) ? maxVert.setX(a) : placeHolder();
            (b>maxVert.y()) ? maxVert.setY(b) : placeHolder();
            (c>maxVert.z()) ? maxVert.setZ(c) : placeHolder();
            // find the minimum vertex
            (a<minVert.x()) ? minVert.setX(a) : placeHolder();
            (b<minVert.y()) ? minVert.setY(b) : placeHolder();
            (c<minVert.z()) ? minVert.setZ(c) : placeHolder();
            
            
            v.set(a, b, c);
            verts[i]=v;
            
        }
        
        for(int j=0; j<polys_count; j++){
            vector<int> poly;
            int s;
            input >> s;
            poly.resize(s);
            Vec center;
            float minX=9999999;
            float minY=9999999;
            float minZ=9999999;
            float maxX=-9999999;
            float maxY=-9999999;
            float maxZ=-9999999;
            for(int k=0; k<s; k++){
                int a;
                input >> a;
                poly[k]=a;
                surround_polys[a].push_back(j);
                (verts[a].x()<minX ? minX=verts[a].x() : 0 );
                (verts[a].y()<minY ? minY=verts[a].y() : 0 );
                (verts[a].z()<minZ ? minZ=verts[a].z() : 0 );
                (verts[a].x()>maxX ? maxX=verts[a].x() : 0 );
                (verts[a].y()>maxY ? maxY=verts[a].y() : 0 );
                (verts[a].z()>maxZ ? maxZ=verts[a].z() : 0 );
                center.set((minX+maxX)/2, (minY+maxY)/2,(minZ+maxZ)/2);
                
            }
            poly_center[j]=center;
            polys[j]=poly;
        }
        
        
        setNormals();
        setCenter();
        setVertexNormals();
        //setTexCoords();
        
        
        
    }
    // Rotation Functions:
    // rotate according to fixed angle
    void rotate(float x=0, float y=0, float z=0,bool h=false){
        Quaternion q=Quaternion::fixedAngle(x, y, z);
        
        //float r[16]={1,0,0,0,0,cosf(angle),-sinf(angle),0,0,sinf(angle),cosf(angle),0,0,0,0,1};
        
        rotate(q,h);
        
        
        
        
    }
    // rotate according to quaternion
    void rotate(Quaternion q,bool h=false){
        
        Matrix M=q.rotMatrix();
        
        //Matrix M=R.multiply(TY);
        //Matrix M=R;
        for(int i=0; i<verts.size();i++){
            float v[4]={verts[i].x(),verts[i].y(),verts[i].z(),1};
            Matrix V(v,4,1);
            Matrix F=M.multiply(V);
            verts[i].set(F.get(0,0)/F.get(3,0), F.get(1,0)/F.get(3,0), F.get(2,0)/F.get(3,0));
            
        }
        {
            float v[4]={minVert.x(),minVert.y(),minVert.z(),1};
            Matrix V(v,4,1);
            Matrix F=M.multiply(V);
            minVert.set(F.get(0,0)/F.get(3,0), F.get(1,0)/F.get(3,0), F.get(2,0)/F.get(3,0));
        }
        {
            float v[4]={maxVert.x(),maxVert.y(),maxVert.z(),1};
            Matrix V(v,4,1);
            Matrix F=M.multiply(V);
            maxVert.set(F.get(0,0)/F.get(3,0), F.get(1,0)/F.get(3,0), F.get(2,0)/F.get(3,0));
        }
        center=(minVert+maxVert)/2;
        //translate(0, center.y()-maxVert.y(), 0,false);
        
        // rotate all of the decendents
        if(h){
            vector<PolyModel*> d=getPolyDecendents();
            for(int j=0; j<d.size();j++){
                d[j]->rotate(q,false);
            }
        }
        
        
        
    }
    
    
    // Rotate To the parent
    void rotate2(float angle){
        
        translate(-attach.x(), -attach.y(), -attach.z());
        rotate(angle);
        translate(parentAttach.x(), parentAttach.y(), parentAttach.z());
        
    }
    // Rotate to the center of model
    void selfRotate(float x,float y,float z,bool h=false){
        Quaternion q=Quaternion::fixedAngle(x, y, z);
        selfRotate(q,h);
    }
    void selfRotate(Quaternion q,bool h=false){
        Vec distance=center;
        translate(-distance.x(), -distance.y(), -distance.z());
        rotate(q,false);
        translate(distance.x(), distance.y(), distance.z());
        if(h){
            vector<PolyModel*> d=getPolyDecendents();
            for(int j=0; j<d.size();j++){
                d[j]->translate(-distance.x(), -distance.y(), -distance.z());
                d[j]->rotate(q,false);
                d[j]->translate(distance.x(), distance.y(), distance.z());
                
            }
            
        }
        
        
    }
    
    void draw(int mode=0, bool h=false)
    {
        
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        //selfRotate(mode);
        
        glBegin(GL_TRIANGLES);
        for (int i=0; i<polys.size(); ++i)
        {
            if (polys[i].size() == 3)
            {
                for (int j=0; j<3; ++j)
                {
                    int idx = polys[i][j];
                    if(idx>=normals.size()){
                        idx-=(normals.size());
                    }
                    Vec v = verts[idx];
                    glNormal3f(normals[idx].x(),normals[idx].y(),normals[idx].z());
                    glVertex3f(v.x(),v.y(),v.z());
                }
            }
        }
        glEnd();
        
        // draw all of the decendents
        if(h){
            vector<PolyModel*> d=getPolyDecendents();
            for(int j=0; j<d.size();j++){
                d[j]->draw(0,false);
            }
        }
        
    }
    void scale(float f){
        for(int i=0; i<verts.size();i++){
            verts[i].change(-center.x(),-center.y(),-center.z());
            verts[i]=verts[i].scale(f);
            verts[i].change(center.x(),center.y(),center.z());
        }
        
        maxVert.change(-center.x(),-center.y(),-center.z());
        maxVert=maxVert.scale(f);
        maxVert.change(center.x(),center.y(),center.z());
        
        
        
        minVert.change(-center.x(),-center.y(),-center.z());
        minVert=minVert.scale(f);
        minVert.change(center.x(),center.y(),center.z());
        
        center=(maxVert+minVert)/2;
        
    }
    
    void scale(float a,float b,float c){
        for(int i=0; i<verts.size();i++){
            verts[i].change(-center.x(),-center.y(),-center.z());
            verts[i]=verts[i].mult(Vec(a,b,c));
            verts[i].change(center.x(),center.y(),center.z());
        }
        maxVert.change(-center.x(),-center.y(),-center.z());
        maxVert=maxVert.mult(Vec(a,b,c));
        maxVert.change(center.x(),center.y(),center.z());
        
        
        
        minVert.change(-center.x(),-center.y(),-center.z());
        minVert=minVert.mult(Vec(a,b,c));
        minVert.change(center.x(),center.y(),center.z());
        
        center=(maxVert+minVert)/2;
        
        
    }
    
    void translate(float x,float y,float z,bool h=true){
        // translate the object itself
        for(int i=0; i<verts.size();i++){
            verts[i].change(x,y,z);
            
        }
        maxVert.change(x,y,z);
        minVert.change(x,y,z);
        center=(maxVert+minVert)/2;
        
        // translate all of the decendents
        if(h){
            vector<PolyModel*> d=getPolyDecendents();
            for(int j=0; j<d.size();j++){
                d[j]->translate(x, y, z,false);
            }
        }
        
        
    }
    
    // Translate the object to x, y, z away from the parent
    void translateToParent(float x,float y, float z){
        float dx=center.x()-PolyParent[0]->center.x()+x;
        float dy=center.y()-PolyParent[0]->center.y()+y;
        float dz=center.z()-PolyParent[0]->center.z()+z;
        for(int i=0; i<verts.size();i++){
            verts[i].change(-dx, -dy, -dz);
        }
        maxVert.change(-dx,-dy,-dz);
        minVert.change(-dx,-dy,-dz);
        center=(maxVert+minVert)/2;
        
    }
    
    
    void placeHolder(){
        
    }
    
    void setCenter(){
        center.set((getMaxVert().x()+getMinVert().x())/2, (getMaxVert().y()+getMinVert().y())/2, (getMaxVert().z()+getMinVert().z())/2);
    }
    
    vector<Vec> getVerts(){
        return verts;
    }
    
    vector<vector<int>> getPolys(){
        return polys;
    }
    
    void setNormals(){
        normals.resize(polys.size());
        for(int i=0; i<polys.size(); i++){
            Vec a=verts[polys[i][2]].sub(verts[polys[i][1]]);
            Vec b=verts[polys[i][1]].sub(verts[polys[i][0]]);
            normals[i]=a.cross(b);
            
        }
    }
    void setVertexNormals(){
        vertex_normal.resize(verts.size());
        for(int i=0; i<verts.size(); i++){
            Vec sum; // the sum of the normals of surrounding polygons
            for(int j=0; j<surround_polys[i].size(); j++){
                sum=sum.add(normals[surround_polys[i][j]]);
                
            }
            Vec avg=sum.normalize(); // the average normals
            vertex_normal[i]=avg;
        }
        
    }
    
    void setTexCoords(){
        tex_coords.resize(verts.size());
        
        // set the radius of the intermediate cylinder texture mapping
        float r=1.5*(getMaxVert().sub(getCenter())).length();
        // set h
        // ball=15, house=3
        float h=15*getMaxVert().z();
        
        for(int i=0; i<verts.size(); i++){
            // theta->z->h
            tex_coords[i].resize(3);
            // nxt+a=rcos(d) ->x
            // nyt+b=rsin(d) ->y
            // nzt+c=z    ->z
            Vec v=getVertNormals()[i];
            float nx=v.x(); // x vertex normal of the vertex
            float ny=v.y();
            float nz=v.z();
            float a=getVerts()[i].x();
            float b=getVerts()[i].y();
            float c=getVerts()[i].z();
            
            // x^2+y^2=r^2
            // ((nx)t+a)^2+((ny)t+b)^2-r^2=0
            float factora=nx*nx+ny*ny;
            float factorb=2*a*nx+2*b*ny;
            float factorc=a*a+b*b-r*r;
            // solve the equation, get the t
            vector<float> result=Util::solveX2(factora, factorb, factorc);
            float t=(result[0]>0) ? result[0] : result[1] ;
            // get theta (d)
            float d=atan((ny*t+b)/(nx*t+a));
            //(d <0)? (d+=2*PI) : d;
            
            // get z
            float z=nz*t+c;
            // set the tex coord
            tex_coords[i][0]=d;
            tex_coords[i][1]=z;
            tex_coords[i][2]=h;
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
        }
        
        
        
    }
    vector<vector<float>> getTexCoords(){
        return tex_coords;
    }
    
    vector<Vec> getNormals(){
        return normals;
    }
    
    vector<Vec> getVertNormals(){
        return vertex_normal;
    }
    
    Vec getMaxVert(){
        return maxVert;
    }
    
    Vec getMinVert(){
        return minVert;
    }
    Vec getCenter(){
        return center;
    }
    vector<Vec> getPolyCenter(){
        return poly_center;
    }
    vector<vector<int>> getSur(){
        return surround_polys;
    }
    
    
    void addPolyChild(PolyModel &m){
        PolyChildren.push_back(&m);
        m.PolyParent.push_back(this);
    }
    
    
    PolyModel* getPolyParent(){
        return PolyParent[0];
    }
    
    vector<PolyModel*> getPolyDecendents(){
        vector<PolyModel*> result;
        
        if(PolyChildren.size()==0){
            //result.push_back(*this);
            
        }
        else{
            for(int i=0; i<PolyChildren.size();i++){
                vector<PolyModel*> d=PolyChildren[i]->getPolyDecendents();
                
                for(int j=0; j<d.size();j++){
                    result.push_back(d[j]);
                }
                result.push_back(PolyChildren[i]);
                
            }
        }
        
        
        
        return result;
    }
    
    
    float getHeight(){
        return maxVert.y()-minVert.y();
    }
    
    Vec attach; // attach point of this model to the parent model
    Vec parentAttach; // the attach point of the parent model to this model
    
    
    
protected:
    vector<Vec> verts;  // Vertex table
    vector<vector<int>> polys;  // Polygon table
    vector<Vec> normals; // the normals of the polygons
    vector<Vec> vertex_normal; // the normal of the verticies
    vector<vector<int>> surround_polys; // the surrounding polygons of verticies
    vector<Vec> poly_center; // the center of each polygon
    vector<vector<float>> tex_coords; // the texture coordinates of each vertex (might not be used in this project)
    Vec maxVert;
    Vec minVert;
    Vec center;
    vector<PolyModel*> PolyParent;
    vector<PolyModel*> PolyChildren;
    
    
};


#endif /* Header_h */
