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
        Light();
        virtual ~Light();

    private:
        Vector4D _position;
        Vector4D _diffuse;
};

#endif /* LIGHT_H_ */
