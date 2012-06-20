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
#include <GL/gl.h>
#include <GL/glu.h>

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



void Scene::render()
{
//    _camera->load();
    
    gluLookAt( 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
    gluPerspective( 60.0, 1.0, 1.0f, 5.0f );
    
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glBegin( GL_TRIANGLES );
    glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
    glVertex4f( 0.0f, 0.0f, 0.0f, 1.0f );
    glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
    glVertex4f( 1.0f, 0.0f, 0.0f, 1.0f );
    glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
    glVertex4f( 0.0f, 1.0f, 0.0f, 1.0f );
    glEnd();
}
