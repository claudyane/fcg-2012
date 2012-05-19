/*
 * Sphere.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"
#include "Material.h"

class Sphere : public Object
{
public:
    Sphere( double centerX, double centerY, double centerZ, double radius );

    virtual ~Sphere();
    bool computeRayIntersection( Ray ray, double& t );
    Vector4D getNormal( Vector4D& point );
    void setMaterial( int materialId );
    int getMaterialId();

private:
    Vector4D _center;
    double _radius;
    int _materialId;
};

#endif /* SPHERE_H_ */
