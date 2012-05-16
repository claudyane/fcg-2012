/*
 * Light.cpp
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#include "Light.h"


Light::Light( Vector4D position, float r, float g, float b )
{
    _position = position;
    _diffuse[0] = r;
    _diffuse[1] = g;
    _diffuse[2] = b;
}



Light::~Light()
{
    //do nothing
}



void Light::getDiffuse(float& r, float& g, float& b)
{
    r = _diffuse[0];
    g = _diffuse[1];
    b = _diffuse[2];
}
