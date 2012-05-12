/*
 * Vector3D.cpp
 *
 *  Created on: May 12, 2012
 *      Author: walther & eliana
 */

#include "Vector4D.h"
#include <cmath>

Vector4D::Vector4D()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;
}



Vector4D::Vector4D( const Vector4D& copy )
{
    x = copy.x;
    y = copy.y;
    z = copy.z;
    w = copy.w;
}



Vector4D::Vector4D( double x, double y, double z, double w )
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}



Vector4D::~Vector4D()
{
}



double Vector4D::norm2()
{
    return x*x + y*y + z*z;
}



double Vector4D::norm()
{
    return sqrt( x*x + y*y + z*z);
}



void Vector4D::normalize()
{
    double normValue = norm();
    x /= normValue;
    y /= normValue;
    z /= normValue;
}



Vector4D Vector4D::operator+=( const Vector4D& a )
{
    x += a.x;
    y += a.y;
    z += a.z;
    return *this;
}



Vector4D Vector4D::operator-=( const Vector4D& a )
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
    return *this;
}



Vector4D Vector4D::operator*=( const double a )
{
    x *= a;
    y *= a;
    z *= a;
    return *this;
}



Vector4D Vector4D::operator/=( const double a )
{
    x /= a;
    y /= a;
    z /= a;
    return *this;
}



Vector4D Vector4D::operator-()
{
    x = -x;
    y = -y;
    z = -z;
    return *this;
}



Vector4D cross( const Vector4D& a, const Vector4D& b )
{
    return Vector4D( a.y*b.z - a.z*b.y,
                     b.x*a.z - b.z*a.x,
                     a.x*b.y - a.y*b.x,
                     1.0 );
}



double dot( const Vector4D& a, const Vector4D& b )
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}



Vector4D operator+( const Vector4D& a, const Vector4D& b )
{
    return Vector4D( a.x + b.x, a.y + b.y, a.z + b.z, 1.0 );
}



Vector4D operator-( const Vector4D& a, const Vector4D& b )
{
    return Vector4D( a.x - b.x, a.y - b.y, a.z - b.z, 1.0 );
}



Vector4D operator*( const double a, const Vector4D& b )
{
    return Vector4D( a*b.x, a*b.y, a*b.z, 1.0 );
}



Vector4D operator*( const Vector4D& a, const double b )
{
    return Vector4D( b*a.x, b*a.y, b*a.z, 1.0 );
}



