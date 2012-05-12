/*
 * Vector3D.h
 *
 *  Created on: May 12, 2012
 *      Author: walther
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

class Vector3D
{
public:
    Vector3D( double x, double y, double z );
    virtual ~Vector3D();
    double getX();
    double getY();
    double getZ();

private:
    double _x;
    double _y;
    double _z;
};

#endif /* VECTOR3D_H_ */
