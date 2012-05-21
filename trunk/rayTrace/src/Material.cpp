/*
 * Material.cpp
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#include "Material.h"

Material::Material( Color diffuse, Color specular, float specularExponent )
{
    _diffuse = diffuse;
    _specular = specular;

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



Color Material::getDiffuse()
{
    return _diffuse;
}



Color Material::getSpecular( float& specularExponent )
{
     specularExponent = _specularExponent;
     return _specular;
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


