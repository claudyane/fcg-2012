/*
 * Camera.cpp
 *
 *  Created on: May 12, 2012
 *      Author: eliana & walther
 */

#include <cmath>

#include "Vector4D.h"
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

    _ze = _eye - _center;
    _ze.normalize();
    _xe = cross( _ze, _up );
    _xe.normalize();
    _ye = cross( _ze, _xe );
}

Ray Camera::computeRay( int x, int y )
{
    Ray ray;

    ray.origin = _eye;
    ray.direction = -_near * _ze + _a * _ye * (((double)y/(double)_height) - 0.5) + _b * _xe * (((double)x/(double)_width)-0.5);

    return ray;
}
