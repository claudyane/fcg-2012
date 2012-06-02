/*
 * Camera.h
 *
 *  Created on: May 12, 2012
 *      Author: eliana & walther
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector4D.h"
#include "Ray.h"

class Camera
{
public:
    /**
     * Constructor
     */
    Camera( Vector4D eye, Vector4D center, Vector4D up, double fovy, double near, int width, int height );

    /**
     * Copy constructor
     */
    Camera( const Camera& copy );

    /**
     * Desctructor
     */
    virtual ~Camera();

    /**
     * Calculates the remaining parameters not provided at construction.
     * This method may also prove useful if the camera suffer any changes during the program.
     */
    void computeDerivedParameters();

    /**
     * Given a pixel coordinates, computeRay will return a ray that starts in camera and passes through the pixel.
     * @param x Horizontal position of the pixel in the screen.
     * @param y Vertical position of the pixel in the screen.
     * @return Ray that starts in the eye position and goes through the pixel.
     */
    Ray computeRay( double x, double y );
    
    /**
     * Returns the screen size
     */
    void getScreenSize( int& width, int& height );
    
    /**
     * Returns the camera's position in world coordinates
     */
    Vector4D getPosition();
    
    void incX( float increment );
    void incY( float increment );
    void incZ( float increment );
    void rotateY( double angle );
    
    void setCenter( Vector4D center );

private:
    /** Position of the camera on world space */
    Vector4D _eye;

    /** Up vector of the camera */
    Vector4D _up;

    /** Point in the world the camera is looking at */
    Vector4D _center;

    Vector4D matrixMult( double matrix[][3], Vector4D point );
    
    /** Field of vision */
    double _fovy;

    /** distance to near plane */
    double _near;

    /** distance to far plane */
    double _far;

    /** width of the screen in pixels */
    int _width;

    /** height of the screen in pixels */
    int _height;

    /** width of screen in world units */
    double _a;

    /** height of screen in world units */
    double _b;

    /** base of the eye space */
    Vector4D _xe;
    Vector4D _ye;
    Vector4D _ze;
};

#endif /* CAMERA_H_ */
