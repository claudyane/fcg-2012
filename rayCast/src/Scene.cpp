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
#include <iostream>

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



Camera* Scene::getCamera()
{
    return _camera;
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
    
    _camera->setCenter( _volume->getCenter() );
    
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
            imgSetPixel3f( image, x, y, pixelColor.r*pixelColor.a, pixelColor.g*pixelColor.a, pixelColor.b*pixelColor.a );
        }
    }
    
    return image;
}



void Scene::computeRayColor( Ray ray, Color& colorOut )
{
    double tIn, tOut;
    if (!_volume->computeRayIntersection( ray, tIn, tOut ))
    {
        colorOut = _backgroundColor;
        return;
    }

    // if there were an intersection, traverse the volume adding colors
    double dt = _volume->getSmallestDimension() / 4.0;
    Color colorNow( 0.0f, 0.0f, 0.0f, 0.0f );
    for( double tCurr = tIn; tCurr <= tOut; tCurr += dt )
    {
        Vector4D point  = ray.origin + tCurr  * ray.direction;
        
        Color colorVoxel = _volume->interpolate( point );
               
        colorNow.r += (1-colorNow.a) * colorVoxel.a * colorVoxel.r;
        colorNow.g += (1-colorNow.a) * colorVoxel.a * colorVoxel.g;
        colorNow.b += (1-colorNow.a) * colorVoxel.a * colorVoxel.b;
        colorNow.a += (1-colorNow.a) * colorVoxel.a;
        
        if( colorNow.a >= 0.95f )
        {
            //colorNow.a = 1.0f;
            break;
        }
    }
    
    colorOut = colorNow;
}
