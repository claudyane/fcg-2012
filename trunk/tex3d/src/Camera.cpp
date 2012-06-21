/*
 * Camera.cpp
 *
 *  Created on: May 12, 2012
 *      Author: eliana & walther
 */

#include <cmath>
#include <GL/glu.h>

#include "Vector4D.h"
#include "Camera.h"
#include "Scene.h"

Camera::Camera( Vector4D eye, Vector4D center, Vector4D up, double fovy, double near, double far, int width, int height )
{
    _eye = eye;
    _center = center;
    _up = up;
    _fovy = fovy;
    _near = near;
    _far = far;
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



void Camera::load()
{   
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( _fovy, (double)_width/(double)_height, _near, _far );
     
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    if(_manipulator)
	{
		_manipulator->SetZCenter( (_center -_eye).norm() );
		_manipulator->Load();
	}
    
    gluLookAt( _eye.x, _eye.y, _eye.z, _center.x, _center.y, _center.z, _up.x, _up.y, _up.z );   
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



Vector4D Camera::getCenter()
{
    return _center;
}



void Camera::incX(float increment)
{
    _eye.x += increment;
    computeDerivedParameters();
}



void Camera::incY(float increment)
{
    _eye.y += increment;
    computeDerivedParameters();
}



void Camera::incZ(float increment)
{
    _eye.z += increment;
    computeDerivedParameters();
}



void Camera::rotateY( double angle )
{
    double matrix[3][3];
    
    matrix[0][0] = cos( angle );  matrix[0][1] = 0.0; matrix[0][2] = sin( angle );
    matrix[1][0] = 0.0;           matrix[1][1] = 1.0; matrix[1][2] = 0.0;
    matrix[2][0] = -sin( angle ); matrix[2][1] = 0.0; matrix[2][2] = cos( angle );
    
    _eye = matrixMult( matrix, _eye );
    computeDerivedParameters();
}



void Camera::setCenter( Vector4D center )
{
    _center = center;
    computeDerivedParameters();
}



void Camera::setManipulator( VManipulator* manipulator )
{
    _manipulator = manipulator;
}



Vector4D Camera::matrixMult( double matrix[][3], Vector4D point )
{
    Vector4D result;
    
    result.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z;
    result.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z;
    result.z = matrix[2][0] * point.x + matrix[2][1] * point.y + matrix[2][2] * point.z;
    
    return result;
}


