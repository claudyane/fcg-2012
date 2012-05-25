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
#include <iostream>

#include <cmath>

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
            return getBoxTextureColor( point, (Box*) object );
            //return _diffuse;
            break;
            
        case Object::SPHERE:
            return getSphericalTextureColor( point, (Sphere*)object );
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
    double radius = sphere->getRadius();
    
//    double s = acos( point.z / radius ) * M_1_PI;
//    double t = acos( point.x / ( radius * sin( M_PI * s ) ) ) * 0.5 * M_1_PI;
    
    Vector4D vn( 0.0, 1.0, 0.0, 1.0 );
    Vector4D ve( 1.0, 0.0, 0.0, 1.0 );
    Vector4D vp = point - sphere->getCenter();
    vp.normalize();
    
    double s = acos( -dot( vn, vp )) / M_PI;
    double t = ( acos( dot( vp, ve ) / sin( M_PI * s )) ) / ( 2 * M_PI);
    
    if ( dot( cross( vn, ve ), vp ) < 0 )
    {
        t = -t;
    }
    
    float r, g, b;
    int width = imgGetWidth( _texture ) - 1;
    int height = imgGetHeight( _texture ) - 1;
    imgGetPixel3f( _texture, (int)( s * width ), (int)(t * height), &r, &g, &b );
    Color textureColor( r, g, b );
    
    return textureColor;
}



Color Material::getBoxTextureColor(Vector4D point, Box* box)
{
    Vector4D min = box->getMin();
    Vector4D max = box->getMax();
    
    float u = 0.0f, v = 0.0f;
    int aux = -1;
    
    // Testa se está no plano x = xmin
    if (fabs(point.x - min.x) < 0.0001)
    {
        aux = 0;
        u = (point.y - min.y)/(max.y - min.y);
        v = (point.z - max.z)/(min.z - max.z);        
    }
    
    // Testa se está no plano x = xmax
    if (fabs(point.x - max.x) < 0.0001)
    {
        aux = 1;
        u = (point.y - min.y)/(max.y - min.y);
        v = (point.z - min.z)/(max.z - min.z);
    }
    
    // Testa se está no plano y = ymin
    if (fabs(point.y - min.y) < 0.0001)
    {
        aux = 2;
        u = (point.x - min.x)/(max.x - min.x);
        v = (point.z - min.z)/(max.z - min.z);    
    }
    
    // Testa se está no plano y = ymax
    if (fabs(point.y - max.y) < 0.0001)
    {
        aux =3;
        u = (point.z - min.z)/(max.z - min.z);
        v = (point.x - min.x)/(max.x - min.x);        
    }       
        
    // Testa se está no plano z = zmin
    if (fabs(point.z - min.z) < 0.0001)
    {
        aux = 4;
        u = (point.y - min.y)/(max.y - min.y);
        v = (point.x - min.x)/(max.x - min.x);        
    }
    
    // Testa se está no plano z = xmax
    if (fabs(point.z - max.z) < 0.0001)
    {
        aux = 5;
        u = (point.x - min.x)/(max.x - min.x);
        v = (point.y - min.y)/(max.y - min.y);        
    }
    
    // Calcula x,y da imagem correspondentes a essa coordenadas de textura
    int width = imgGetWidth( _texture );
    int height = imgGetHeight( _texture );
    
    int x = (int)(u * (width - 1)) % width;
    int y = (int)(v * (height - 1))% height;
     
    // Recupera a cor da imagem neste ponto
    float r, g, b;
    imgGetPixel3f( _texture, x, y, &r, &g, &b );
    
    return Color( r, g, b );
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
