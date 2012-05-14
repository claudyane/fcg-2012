/*
 * Sphere.cpp
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#include "Sphere.h"
#include "Object.h"

Sphere::Sphere( double centerX, double centerY, double centerZ, double radius )
{
    _center.x = centerX;
    _center.y = centerY;
    _center.z = centerZ;
    _radius = radius;
}

Sphere::~Sphere()
{
}

bool Sphere::computeRayIntersection( Ray ray, Vector4D & point )
{
    return false;
}
