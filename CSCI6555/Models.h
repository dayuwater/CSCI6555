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
#include "Vector.h"
#include "Util.h"




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
        children.push_back(m);
        m.parent.push_back(*this);
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
    
    Model getParent(){
        return parent[0];
    }
    
    vector<Model> getDecendents(){
        vector<Model> result;
        
        if(children.size()==0){
            //result.push_back(*this);
            
        }
        else{
            for(int i=0; i<children.size();i++){
                vector<Model> d=children[i].getDecendents();
                
                for(int j=0; j<d.size();j++){
                    result.push_back(d[j]);
                }
                result.push_back(children[i]);
                
            }
        }
        
        
        
        return result;
    }
    
    
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
protected:
    vector<Model> parent;
    vector<Model> children;
    
    
    
};

class Cube : public Model{
public:
    Cube(float sizee=0.5f,float x=0.0f,float y=0.0f,float z=-5.0f){
        size=sizee;
        _x=x;
        _y=y;
        _z=z;
    }
    float size;
    
    void draw(int mode=0){
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        selfRotate(mode);
        glutSolidCube(size);
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
    void draw(int mode=0)
    {
        
        glLoadIdentity();
        glTranslatef(_x, _y, _z);
        selfRotate(mode);
        
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
            (d <0)? d+=2*Util::Constants::PI : d;
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
