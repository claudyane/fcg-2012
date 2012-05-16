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

    _material = 0;
}



Sphere::~Sphere()
{
}


bool Sphere::computeRayIntersection( Ray ray, Vector4D & point, Vector4D& normal )
{
    double a = dot( ray.direction, ray.direction );
    double b = dot( 2 * ray.direction, ray.origin - _center );
    double c = dot( ray.origin - _center, ray.origin - _center ) - (_radius * _radius);
    
    double delta = ( b * b ) - 4 * a * c; //b² - 4ac
    if( delta < 0 ) return false; //if there are no real solutions, there's no intersection
    
    double t = ( -b - sqrt( delta ) ) / ( 2 * a );
    
    if (t < 0)
    {
        t = ( -b + sqrt( delta ) ) / ( 2 * a );
        if (t < 0) return false; //if t is negative the intersection was behind the camera
    }
    
    point = ray.origin + ( t * ray.direction );
    
    normal = point - _center;
    normal.normalize();
    
    return true;
}



void Sphere::getColor( Vector4D& point, float& r, float& g, float& b, float& a )
{
    if ( point.norm2() <= ( _radius * _radius ) )
    {
        _material->getDiffuse( r, g, b );
        a = _material->getOpacity();
    }
    else
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 0.0f;
    }
}



void Sphere::setMaterial( Material* material )
{
    _material = material;
}