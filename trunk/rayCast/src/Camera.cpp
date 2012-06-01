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



Camera::Camera( const Camera& copy )
{
    this->_eye    = copy._eye;
    this->_up     = copy._up;
    this->_center = copy._center;
    this->_fovy   = copy._fovy;
    this->_near   = copy._near;
    this->_far    = copy._far;
    this->_width  = copy._width;
    this->_height = copy._height;
    this->_a      = copy._a;
    this->_b      = copy._b;
    this->_xe     = copy._xe;
    this->_ye     = copy._ye;
    this->_ze     = copy._ze;
}



Camera::~Camera()
{
    // TODO Auto-generated destructor stub
}



void Camera::computeDerivedParameters()
{
    _a = 2.0 * _near * tan( ((_fovy * M_PI)/180.0) / 2.0 );
    _b = _a * (double) _width / (double) _height;

    _ze = _eye - _center;
    _ze.normalize();
    _xe = cross( _up, _ze );
    _xe.normalize();
    _ye = cross( _ze, _xe );
}



Ray Camera::computeRay( double x, double y )
{
    Ray ray;

    ray.origin = _eye;
    ray.direction = -_near * _ze + _a * _ye * ((y/(double)_height) - 0.5) + _b * _xe * ((x/(double)_width)-0.5);

    return ray;
}



void Camera::getScreenSize( int& width, int& height )
{
    width = _width;
    height= _height;
}



Vector4D Camera::getPosition()
{
    return _eye;
}



void Camera::incX(float increment)
{
    _eye.x += increment;
}

void Camera::incY(float increment)
{
    _eye.y += increment;
}

void Camera::incZ(float increment)
{
    _eye.z += increment;
}
