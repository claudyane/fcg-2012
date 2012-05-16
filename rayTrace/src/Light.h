/*
 * Light.h
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Vector4D.h"

class Light
{
    public:
        Light( Vector4D position, float r, float g, float b );
        virtual ~Light();
        
    private:

        Vector4D _position;
        float _diffuse[3];
};

#endif /* LIGHT_H_ */
