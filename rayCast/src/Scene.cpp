/* 
 * File:   Scene.cpp
 * Author: walther
 * 
 * Created on May 28, 2012, 5:00 PM
 */

#include "Scene.h"
#include "Image.h"

Scene::Scene()
{
    _volume = NULL;
    _camera = NULL;
}

Scene::~Scene()
{
    delete _volume;
    delete _camera;
}

void Scene::setCamera( Camera* camera )
{
    _camera = camera;
}

void Scene::setVolume( Volume* volume )
{
    _volume = volume;
}

Image* Scene::render()
{
    int width, height;
    _camera->getScreenSize( width, height );
    
    Image* image = imgCreate( width, height, 3 );
    
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            Ray ray = _camera->computeRay( x + 0.25, y + 0.25 );
            Color pixelColor;
            computeRayColor( ray, pixelColor );
            imgSetPixel3f( image, x, y, pixelColor.r, pixelColor.g, pixelColor.b );
        }
    }
    
    return image;
}


void Scene::computeRayColor( Ray ray, Color& colorOut )
{
    double tIn, tOut;
    
    if (_volume->computeRayIntersection( ray, tIn, tOut ))
    {
        Vector4D pIn  = ray.origin + tIn  * ray.direction;
        Vector4D pOut = ray.origin + tOut * ray.direction;

        colorOut.set( 1.0f, 0.0f, 0.0f, 1.0f );
    }
    else
    {
        colorOut = _backgroundColor;
    }
}