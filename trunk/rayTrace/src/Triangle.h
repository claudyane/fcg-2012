/* 
 * File:   Triangle.h
 * Author: eliana
 *
 * Created on May 19, 2012, 1:29 PM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include "Material.h"
#include "Vector4D.h"
#include "Ray.h"
#include "Object.h"

class Triangle : public Object
{
public:
    
    Triangle( Vector4D v0, Vector4D v1, Vector4D v2, Vector4D tex0, Vector4D tex1, Vector4D tex2 );
    
    virtual ~Triangle();
    
    bool computeRayIntersection( Ray ray, double& t );
    
    Vector4D getNormal( Vector4D& point );
    
    void setMaterial( int materialId );
    
    int getMaterialId();
    
private:
    
    int _materialId;
    
    Vector4D _v0;
    Vector4D _v1;
    Vector4D _v2;
    
    Vector4D _tex0;
    Vector4D _tex1;
    Vector4D _tex2;
};

#endif	/* TRIANGLE_H */

