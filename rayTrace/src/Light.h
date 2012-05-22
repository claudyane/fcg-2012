/*
 * Light.h
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Vector4D.h"
#include "Color.h"

class Light
{
    public:
        /**
         * Constructor
         * @param position Vector4D with position of the light in world. In World coordinates.
         * @param r red value of the diffuse color.
         * @param g green value of the diffuse color.
         * @param b blue value of the diffuse color.
         */
        Light( Vector4D position, float r, float g, float b );
        
        /**
         * Destructor
         */
        virtual ~Light();
                
        /**
         * Getter for the diffuse color
         * @return Color of the light
         */
        Color getDiffuse();
        
        Vector4D getPosition();
        
    private:

        /** position of the light in the world. In world coordinates */
        Vector4D _position;
        
        /** diffuse color of the light */
        float _diffuse[3];
        Color _color;
};

#endif /* LIGHT_H_ */
