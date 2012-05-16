/*
 * Light.cpp
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#include "Light.h"


Light::Light( Vector4D position, Vector4D diffuse )
{
    _position = position;
    _diffuse  = diffuse;
}



Light::~Light()
{
    // TODO Auto-generated destructor stub
}

