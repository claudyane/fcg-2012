/*
 * Sphere.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"

class Sphere : public Object
{
public:
    Sphere( double centerX, double centerY, double centerZ, double radius );
    virtual ~Sphere();
    bool computeRayIntersection( Ray ray, Vector4D& point );

private:
    Vector4D _center;
    double _radius;
};

#endif /* SPHERE_H_ */
