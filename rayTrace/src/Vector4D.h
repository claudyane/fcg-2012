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

	private:

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
 * Operators
 */
Vector4D operator+( const Vector4D& a, const Vector4D& b );
Vector4D operator-( const Vector4D& a, const Vector4D& b );
Vector4D operator*( const double a, const Vector4D& b );
Vector4D operator*( const Vector4D& a, const double b );

#endif /* VECTOR4D_H_ */
