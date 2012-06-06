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

#define LIGHTING

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
        #pragma omp parallel for
        for (int y = 0; y < height; ++y)
        {
            Ray ray = _camera->computeRay( x, y );
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
        
        #ifdef LIGHTING
        Color colorVoxel( 0.0f, 0.0f, 0.0f, 0.0f );        
        
        // Direção de iluminação
        Vector4D lightDir = _camera->getPosition() - point;
        lightDir.normalize();
        
        // Normal do voxel
        Vector4D normal = _volume->getNormal( point );        
        if (normal.norm() > 0.0)
        {
            normal.normalize();
            double cos = dot( normal, lightDir );
            
            if (cos < 0.0)
                cos = -cos;
            
            if (cos > 0.0)
            {
                Color ambient( 0.3f, 0.3f, 0.3f, 1.0f );
                Color diffuse( 0.7f, 0.7f, 0.7f, 1.0f );
                colorVoxel = _volume->interpolate( point );
                float alpha = colorVoxel.a;
                
                colorVoxel = colorVoxel*ambient + diffuse*colorVoxel*cos;
                colorVoxel.a = alpha;
            }
        }
        #else
        Color colorVoxel = _volume->interpolate( point );
        #endif

//        colorNow.r += (1-colorNow.a) * colorVoxel.r * colorVoxel.a;
//        colorNow.g += (1-colorNow.a) * colorVoxel.g * colorVoxel.a;
//        colorNow.b += (1-colorNow.a) * colorVoxel.b * colorVoxel.a;
        colorNow.r += (1-colorNow.a) * colorVoxel.r;
        colorNow.g += (1-colorNow.a) * colorVoxel.g;
        colorNow.b += (1-colorNow.a) * colorVoxel.b;        
        colorNow.a += (1-colorNow.a) * colorVoxel.a;
        
        if( colorNow.a >= 0.95f )
        {
            //colorNow.a = 1.0f;
            break;
        }
    }
    

    
    colorOut = colorNow;
}

