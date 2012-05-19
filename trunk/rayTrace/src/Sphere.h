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
    void getColor( float& r, float& g, float& b, float& a );
    void setMaterial( Material* material );

private:
    Vector4D _center;
    double _radius;
    Material* _material;
};

#endif /* SPHERE_H_ */
