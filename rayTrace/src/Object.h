/*
 * Object.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Vector4D.h"
#include "Ray.h"

class Object
{
public:
    virtual ~Object(){}
    virtual bool computeRayIntersection( Ray ray, double& t ) = 0;
    virtual Vector4D getNormal( Vector4D& point ) = 0;
    virtual void getColor( float& r, float& g, float& b, float& a ) = 0;
};

#endif /* OBJECT_H_ */
