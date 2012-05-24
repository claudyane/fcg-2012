/*
 * Material.h
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Image.h"
#include "Color.h"
#include "Object.h"

//forward declarations
class Sphere;
class Box;
class Triangle;

class Material
{
    public:

        /**
         * Constructor
         */
        Material( Color diffuse, Color specular, float specularExponent );

        /**
         * Destructor
         */
        virtual ~Material();

        /**
         * Returns material's diffuse color
         */
        Color getDiffuse( Object* object, Vector4D& point );

        /**
         * Returns material's specular color and exponent
         */
        Color getSpecular( float& specularExponent );

        /**
         * Returns material's reflection factor
         */
        float getReflectionFactor();

        /**
         * Define material's reflection factor
         */
        void setReflectionFactor( float reflectionFactor );

        /**
         * Returns material's refraction factor
         */
        float getRefractionFactor();

        /**
         * Define material's refraction factor
         */
        void setRefractionFactor( float refractionFactor );

        /**
         * Returns material's opacity
         */
        float getOpacity();

        /**
         * Define material's opacity
         */
        void setOpacity( float opacity );

        /**
         * Returns material's texture image
         */
        Image* getTextureImage();

        /**
         * Define material's texture image
         */
        void setTextureImage( std::string filename );
        
    private:

        Color getSphericalTextureColor( Vector4D point, Sphere* sphere );
        
        Color getBoxTextureColor( Vector4D point, Box* box );
        
        Color getTriangleTextureColor( Vector4D point, Triangle* triangle );
        
        /** Diffuse color of the material */
        Color _diffuse;

        /** Specular color of the material */
        Color _specular;

        /** Specular exponent to control material's shininess */
        float _specularExponent;

        /** Reflection coeficient of the material */
        float _reflectionFactor;

        /** Refraction coeficient of the material */
        float _refractionFactor;

        /** Material's opacity */
        float _opacity;

        /** Pointer to the texture image of the material - NULL if there isn't any */
        Image* _texture;

};

#endif /* MATERIAL_H_ */
