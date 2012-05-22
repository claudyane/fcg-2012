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
    _color.set( r, g, b );
}



Light::~Light()
{
    //do nothing
}



void Light::getDiffuse(float& r, float& g, float& b)
{
    r = _color.r;
    g = _color.g;
    b = _color.b;
}



Color Light::getDiffuse()
{
    return _color;
}



Vector4D Light::getPosition()
{
    return _position;
}
