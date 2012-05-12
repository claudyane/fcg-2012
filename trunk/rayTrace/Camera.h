/*
 * Camera.h
 *
 *  Created on: May 12, 2012
 *      Author: eliana & walther
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector3D.h"

class Camera
{
public:
    /**
     * Constructor
     */
    Camera();

    /**
     * Desctructor
     */
    virtual ~Camera();

private:
    /** Position of the camera on world space */
    Vector3D _eye;

    /** Up vector of the camera */
    Vector3D _up;

    /** Point in the world the camera is looking at */
    Vector3D _center;

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
    double a;

    /** height of screen in world units */
    double b;

    /** base of the eye space */
    Vector3D _eyeBase;
};

#endif /* CAMERA_H_ */
