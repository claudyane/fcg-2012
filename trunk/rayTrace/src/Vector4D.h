/*
 * Vector4D.h
 *
 *  Created on: May 12, 2012
 *      Author: walther & eliana
 */

#ifndef VECTOR4D_H_
#define VECTOR4D_H_

class Vector4D
{
public:

    /**
     * Standard constructor
     * Initializes the (0, 0, 0, 1) vector
     */
    Vector4D();

    /**
     * Copy constructor
     */
    Vector4D( const Vector4D& copy );

    /**
     * Constructor
     * @param[in] x Vector's x coordinate
     * @param[in] y Vector's y coordinate
     * @param[in] z Vector's z coordinate
     */
    Vector4D( double x, double y, double z, double w );

    /**
     * Destructor
     */
    virtual ~Vector4D();

    /**
     * Sets the x, y, z and w values of the vector.
     * @param x Values of the x coordinate
     * @param y Values of the y coordinate
     * @param z Values of the z coordinate
     * @param w Values of the w coordinate
     */
    void set( double x, double y, double z, double w );
    
    /**
     * Returns squared norm
     */
    double norm2();

    /**
     * Return norm
     */
    double norm();

    /**
     * Normalizes the vector
     */
    void normalize();

    /**
     * Operators
     */

    Vector4D operator=( const Vector4D& a );
    Vector4D operator+=( const Vector4D& a );
    Vector4D operator-=( const Vector4D& a );
    Vector4D operator*=( const double a );
    Vector4D operator/=( const double a );
    Vector4D operator-();

    /**
     * Vector's coordinates
     */
    double x;
    double y;
    double z;
    double w;   
};

/**
 * Compute cross operation
 */
Vector4D cross( const Vector4D& a, const Vector4D& b );

/**
 * Compute dot operation
 */
double dot( const Vector4D& a, const Vector4D& b );

/**
 * Computes de reflection of a vector
 * @param normal Axis in which the incident vector will be reflected
 * @param in Incident vector that will be reflected
 * @return Reflected vector
 */
Vector4D reflect( const Vector4D& normal, const Vector4D& in );

/**
 * Operators
 */
Vector4D operator+( const Vector4D& a, const Vector4D& b );
Vector4D operator-( const Vector4D& a, const Vector4D& b );
Vector4D operator*( const double a, const Vector4D& b );
Vector4D operator*( const Vector4D& a, const double b );

#endif /* VECTOR4D_H_ */
