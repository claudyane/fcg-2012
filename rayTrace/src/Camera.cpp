/*
 * Camera.cpp
 *
 *  Created on: May 12, 2012
 *      Author: eliana & walther
 */

#include <cmath>

#include "Camera.h"

Camera::Camera( Vector4D eye, Vector4D center, Vector4D up, double fovy, double near, int width, int height )
{
    _eye = eye;
    _center = center;
    _up = up;
    _fovy = fovy;
    _near = near;
    _far = 2 * near;
    _width = width;
    _height = height;

    computeDerivedParameters();
}

Camera::~Camera()
{
    // TODO Auto-generated destructor stub
}

void Camera::computeDerivedParameters()
{
    _a = 2.0 * _near * tan( _fovy / 2.0 );
    _b = _a * (double) _width / (double) _height;
    //TODO xe, ye, ze
}

Ray Camera::computeRay( int x, int y )
{
    Ray ray;

    ray.origin = _eye;

    return ray;
}
