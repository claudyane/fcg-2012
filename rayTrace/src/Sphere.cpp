/*
 * Sphere.cpp
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#include <cmath>
#include "Sphere.h"
#include "Object.h"


Sphere::Sphere( double centerX, double centerY, double centerZ, double radius )
{
    _center.x = centerX;
    _center.y = centerY;
    _center.z = centerZ;
    _radius = radius;

    _materialId = 0;
}



Sphere::~Sphere()
{
}


bool Sphere::computeRayIntersection( Ray ray, double& t )
{
    double a = dot( ray.direction, ray.direction );
    double b = dot( 2 * ray.direction, ray.origin - _center );
    double c = dot( ray.origin - _center, ray.origin - _center ) - (_radius * _radius);
    
    double delta = ( b * b ) - 4 * a * c; //b² - 4ac
    if( delta < 0 ) return false; //if there are no real solutions, there's no intersection
    
    t = ( -b - sqrt( delta ) ) / ( 2 * a );
    
    if (t < 0)
    {
        t = ( -b + sqrt( delta ) ) / ( 2 * a );
        if (t < 0) return false; //if t is negative the intersection was behind the camera
    }
    
    return true;
}



Vector4D Sphere::getNormal( Vector4D& point )
{
    Vector4D normal = point - _center;
    normal.normalize();
    return normal;
}



void Sphere::setMaterialId( int materialId )
{
    _materialId = materialId;
}



int Sphere::getMaterialId()
{
    return _materialId;
}
