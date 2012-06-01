/* 
 * File:   Scene.cpp
 * Author: walther
 * 
 * Created on May 28, 2012, 5:00 PM
 */

#include "Scene.h"
#include "Image.h"
#include "DefFileReader.h"
#include <cstdio>

Scene::Scene()
{
    _volume = NULL;
    _camera = NULL;    
}



Scene::~Scene()
{
    if (_volume)
        delete _volume;
    
    if (_camera)
        delete _camera;
}



void Scene::setCamera( Camera* camera )
{
    _camera = camera;
}



bool Scene::loadScene( std::string filePath )
{
    char cameraFile[256];
    char volumeFile[256];

    FILE* fp = fopen( filePath.c_str(), "r");
    if (!fp) 
        return 0;
    
    fscanf( fp, "%s\n", cameraFile );
    fscanf( fp, "%s\n", volumeFile );
    fclose( fp );
        
    DefFileReader reader;
    
    size_t pos = filePath.find_last_of( "/\\" );          
    std::string cameraPath = filePath.substr( 0, pos + 1 );
    cameraPath += cameraFile;
    
    if (_camera)
        delete _camera;
    _camera = reader.loadCamera( cameraPath );
    
    std::string volumePath = filePath.substr( 0, pos + 1 );
    volumePath += volumeFile;
    if (_volume)
        delete _volume;
    _volume = reader.loadVolume( volumePath );
    
    return true;
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
        
        Vector4D normal = _volume->getNormal( pIn );
        
        colorOut.set( 0.5, 0.5, 0.5, 1.0f );
        
        //RED
        if( normal.x == 1.0 && normal.y == 0.0 && normal.z == 0.0 )
        {
            colorOut.set( 1.0,0.0,0.0,1.0 );
        }
        //green
        if( normal.x == 0.0 && normal.y == 1.0 && normal.z == 0.0 )
        {
            colorOut.set( 0.0,1.0,0.0,1.0 );
        }
        //blue
        if( normal.x == 0.0 && normal.y == 0.0 && normal.z == 1.0 )
        {
            colorOut.set( 0.0,0.0,1.0,1.0 );
        }
        //cyan
        if( normal.x == -1.0 && normal.y == 0.0 && normal.z == 0.0 )
        {
            colorOut.set( 0.0,1.0,1.0,1.0 );
        }
        //magenta
        if( normal.x == 0.0 && normal.y == -1.0 && normal.z == 0.0 )
        {
            colorOut.set( 1.0,0.0,1.0,1.0 );
        }
        //yellow
        if( normal.x == 0.0 && normal.y == 0.0 && normal.z == -1.0 )
        {
            colorOut.set( 1.0,1.0,0.0,1.0 );
        }
    }
    else
    {
        colorOut = _backgroundColor;
    }
}
