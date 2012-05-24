/*
 * Material.cpp
 *
 *  Created on: May 15, 2012
 *      Author: eliana
 */

#include "Material.h"
#include "Object.h"
#include "Sphere.h"
#include "Box.h"
#include "Triangle.h"

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



Color Material::getDiffuse( Object* object, Vector4D& point )
{
    if( !_texture )
    {
        return _diffuse;
    }
    
    Object::ObjectType objectType = object->getType();
    
    switch( objectType )
    {
        case Object::BOX:
            //do box
            return _diffuse;
            break;
            
        case Object::SPHERE:
            //do sphere
            return _diffuse;
            break;
            
        case Object::TRIANGLE:
            return getTriangleTextureColor( point, (Triangle*) object );
            //return _diffuse;
            break;
            
        default:
            // WHAT SORCERY IS THAT?
            return _diffuse;
            break;
    }
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
    if (filename != "null")
        _texture = imgReadBMP( filename.c_str() );
}



Color Material::getSphericalTextureColor(Vector4D point, Sphere* sphere)
{
    
}



Color Material::getBoxTextureColor(Vector4D point, Box* box)
{
    
}



Color Material::getTriangleTextureColor(Vector4D point, Triangle* triangle)
{
    // Recupera coordenadas de textura dos vertices
    Vector4D tex0, tex1, tex2;
    triangle->getTextureCoordinates( tex0, tex1, tex2 );

    // Recupera coordenadas dos vertices
    Vector4D v0, v1, v2;
    triangle->getVertices( v0, v1, v2 );    
    
    // Calcula coordenadas baricentricas
    float a0 = cross( v1 - point, v2 - point).norm() / 2.0f;
    float a1 = cross( point - v0, v2 - v0).norm() / 2.0f;
    float a2 = cross( v1 - v0, point - v0).norm() / 2.0f;
    float at = cross( v1 - v0, v2 - v0).norm() / 2.0f;
            
    float l0 = a0/at;
    float l1 = a1/at;
    float l2 = a2/at;
    
    // Calcula coordenadas de textura a partir das coordenadas baricentricas
    Vector4D texCoord = l0 * tex0 + l1 * tex1 + l2 * tex2;
    
    // Calcula x,y da imagem correspondentes a essa coordenadas de textura
    int width = imgGetWidth( _texture );
    int height = imgGetHeight( _texture );
    
    int x = (int)(texCoord.x * (width - 1 )) % width;
    int y = (int)(texCoord.y * (height - 1 ))% height;
    
    // Recupera a cor da imagem neste ponto
    Color diffuse;
    imgGetPixel3f( _texture, x, y, &diffuse.r, &diffuse.g, &diffuse.b );
    
    return diffuse;
}
