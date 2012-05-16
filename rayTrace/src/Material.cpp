/*
 * Material.cpp
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#include "Material.h"

Material::Material( float diffuseRed, float diffuseGreen, float diffuseBlue, float specularRed, float specularGreen, float specularBlue, float specularExponent )
{
    _diffuse[0] = diffuseRed;
    _diffuse[1] = diffuseGreen;
    _diffuse[2] = diffuseBlue;

    _specular[0] = specularRed;
    _specular[1] = specularGreen;
    _specular[2] = specularBlue;

    _specularExponent = specularExponent;

    _opacity = 1.0f;
    _reflectionFactor = 0.0f;
    _refractionFactor = 0.0f;
    _texture = 0;

}



Material::~Material()
{
    if (_texture)
    {
        imgDestroy( _texture );
        _texture = 0;
    }
}



void Material::getDiffuse( float& diffuseRed, float& diffuseGreen, float& diffuseBlue )
{
    diffuseRed   = _diffuse[0];
    diffuseGreen = _diffuse[1];
    diffuseBlue  = _diffuse[2];
}



void Material::getSpecular( float& specularRed, float& specularGreen, float& specularBlue, float& specularExponent )
{
    specularRed   = _specular[0];
    specularGreen = _specular[1];
    specularBlue  = _specular[2];

     specularExponent = _specularExponent;
}



float Material::getReflectionFactor()
{
    return _reflectionFactor;
}



void Material::setReflectionFactor( float reflectionFactor )
{
    _reflectionFactor = reflectionFactor;
}



float Material::getRefractionFactor()
{
    return _refractionFactor;
}



void Material::setRefractionFactor( float refractionFactor )
{
    _refractionFactor = refractionFactor;
}



float Material::getOpacity()
{
    return _opacity;
}



void Material::setOpacity( float opacity )
{
    _opacity = opacity;
}



Image* Material::getTextureImage()
{
    return _texture;
}



void Material::setTextureImage( std::string filename )
{
    //_texture = imgReadBMP( filename.c_str() );
}


