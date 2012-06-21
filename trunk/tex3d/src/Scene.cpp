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
    _textureData = 0;
    
    _nSlices = 30;
}



Scene::~Scene()
{
    if (_volume)
        delete _volume;
    
    if (_camera)
        delete _camera;           
    
    if (_textureData)
    {
        glDeleteTextures( 1, &_textureID );
        delete[] _textureData;
    }
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
    
    loadTexture3D();
    
    return true;
}


void Scene::setVolume( Volume* volume )
{
    _volume = volume;
}



void Scene::render()
{
    if (!_camera)
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    
        return;
    }
    
    _camera->load();
    
    // Salva parametros do openGL
    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    
    // Desenha a caixa envolvente
    drawBox();
    
    // TODO: resolver numero de fatias
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    // Define orientação das fatias
    float modelview[16];
    glGetFloatv( GL_MODELVIEW_MATRIX , modelview );
    int maxIndex = max( fabs(modelview[8]), fabs(modelview[9]), fabs(modelview[10]) );        
    
    if (maxIndex == 0)
        drawXSlices( nx );
    else if (maxIndex == 1)
        drawYSlices( ny );
    else
        drawZSlices( nz );
    
    glPopAttrib();
}



void Scene::loadTexture3D()
{
    //glClearColor(1.0f,1.0f,1.0f,1.0f);
    if (_textureData)
    {
        glDeleteTextures( 1, &_textureID );
        delete[] _textureData;
        _textureData = 0;
    }

    glGenTextures( 1, &_textureID );
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    
    int width, height, depth;
    _volume->getNumberOfSamples( width, height, depth );
    _textureData =  _volume->getTexture3D();
    GLvoid* data = (GLvoid*) _textureData;
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_FLOAT, data);
}



void Scene::drawXSlices( int num )
{
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    float dx, dy, dz;
    _volume->getVoxelDimension( dx, dy, dz );
    
    float xMax, yMax, zMax;
    xMax = nx*dx;
    yMax = ny*dy;
    zMax = nz*dz;
    
    float positionStep = xMax / num;
    float textureStep = 1.0 / num;
    
    glPushAttrib( GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glEnable( GL_TEXTURE_3D );    
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    for( int slice = 0; slice <= num; ++slice )
    {
        float xPos = positionStep * slice;
        float texPos = textureStep * slice;
        glTexCoord3f( texPos, 0.0f, 0.0f ); glVertex3f( xPos, 0.0f, 0.0f );
        glTexCoord3f( texPos, 0.0f, 1.0f ); glVertex3f( xPos, 0.0f, zMax );
        glTexCoord3f( texPos, 1.0f, 1.0f ); glVertex3f( xPos, yMax, zMax );
        glTexCoord3f( texPos, 1.0f, 0.0f ); glVertex3f( xPos, yMax, 0.0f );
    }
    
    glEnd();
    
    glPopAttrib();
}



void Scene::drawYSlices( int num )
{
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    float dx, dy, dz;
    _volume->getVoxelDimension( dx, dy, dz );
    
    float xMax, yMax, zMax;
    xMax = nx*dx;
    yMax = ny*dy;
    zMax = nz*dz;
    
    float positionStep = yMax / num;
    float textureStep = 1.0 / num;
    
    glPushAttrib( GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glEnable( GL_TEXTURE_3D );    
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    for( int slice = 0; slice <= num; ++slice )
    {
        float yPos = positionStep * slice;
        float texPos = textureStep * slice;
        glTexCoord3f( 0.0f, texPos, 0.0f ); glVertex3f( 0.0f, yPos, 0.0f );
        glTexCoord3f( 1.0f, texPos, 1.0f ); glVertex3f( xMax, yPos, zMax );
        glTexCoord3f( 1.0f, texPos, 1.0f ); glVertex3f( xMax, yPos, zMax );
        glTexCoord3f( 0.0f, texPos, 0.0f ); glVertex3f( 1.0f, yPos, 0.0f );
    }
    
    glEnd();
    
    glPopAttrib();
}



void Scene::drawZSlices( int num )
{
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    float dx, dy, dz;
    _volume->getVoxelDimension( dx, dy, dz );
    
    float xMax, yMax, zMax;
    xMax = nx*dx;
    yMax = ny*dy;
    zMax = nz*dz;
    
    float positionStep = zMax / num;
    float textureStep = 1.0 / num;
    
    glPushAttrib( GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glEnable( GL_TEXTURE_3D );    
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    for( int slice = 0; slice <= num; ++slice )
    {
        float zPos = positionStep * slice;
        float texPos = textureStep * slice;
        glTexCoord3f( 0.0f, 0.0f, texPos ); glVertex3f( 0.0f, 0.0f, zPos );
        glTexCoord3f( 0.0f, 1.0f, texPos ); glVertex3f( 0.0f, yMax, zPos );
        glTexCoord3f( 1.0f, 1.0f, texPos ); glVertex3f( xMax, yMax, zPos );
        glTexCoord3f( 1.0f, 0.0f, texPos ); glVertex3f( xMax, 0.0f, zPos );
    }
    
    glEnd();
    
    glPopAttrib();
}



void Scene::drawSlice()
{
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    float dx, dy, dz;
    _volume->getVoxelDimension( dx, dy, dz );
    
    float xMax, yMax, zMax;
    xMax = nx*dx;
    yMax = ny*dy;
    zMax = nz*dz;
    
    glPushAttrib( GL_TEXTURE_BIT | GL_ENABLE_BIT );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    
    glEnable( GL_TEXTURE_3D );    
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        
    glBegin( GL_QUADS );
    
    glTexCoord3f( 0.0f, 0.0f, 0.5f ); glVertex3f( 0.0f, 0.0f, zMax/2 );
    glTexCoord3f( 0.0f, 1.0f, 0.5f ); glVertex3f( 0.0f, yMax, zMax/2 );
    glTexCoord3f( 1.0f, 1.0f, 0.5f ); glVertex3f( xMax, yMax, zMax/2 );
    glTexCoord3f( 1.0f, 0.0f, 0.5f ); glVertex3f( xMax, 0.0f, zMax/2 );
    
    glEnd();
    
    glDisable( GL_TEXTURE_3D );
    
    glPopAttrib();
}



void Scene::drawBox()
{
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    float dx, dy, dz;
    _volume->getVoxelDimension( dx, dy, dz );
    
    float xMax, yMax, zMax;
    xMax = nx*dx;
    yMax = ny*dy;
    zMax = nz*dz;
    
    glPushAttrib( GL_POLYGON_BIT );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
    glBegin( GL_QUADS );
    
    // x = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, zMax );
    glVertex3f( 0.0f, yMax, zMax );
    glVertex3f( 0.0f, yMax, 0.0f );
    
    // x = xMax
    glVertex3f( xMax, 0.0f, 0.0f );
    glVertex3f( xMax, yMax, 0.0f );
    glVertex3f( xMax, yMax, zMax );
    glVertex3f( xMax, 0.0f, zMax );
    
    // y = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( xMax, 0.0f, 0.0f );
    glVertex3f( xMax, 0.0f, zMax );
    glVertex3f( 0.0f, 0.0f, zMax );
    
    // y = yMax
    glVertex3f( 0.0f, yMax, 0.0f );
    glVertex3f( 0.0f, yMax, zMax );
    glVertex3f( xMax, yMax, zMax );
    glVertex3f( xMax, yMax, 0.0f );
    
    // z = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, yMax, 0.0f );
    glVertex3f( xMax, yMax, 0.0f );
    glVertex3f( xMax, 0.0f, 0.0f );
    
    // z = zMax
    glVertex3f( 0.0f, 0.0f, zMax );
    glVertex3f( xMax, 0.0f, zMax );
    glVertex3f( xMax, yMax, zMax );
    glVertex3f( 0.0f, yMax, zMax );
    
    glEnd();
    
    glPopAttrib();
}



int Scene::max(float a, float b, float c)
{
    if (a > b)
    {
        if (a > c)
            return 0; // a é o maior
    }
    else
    {
        if (b > c)
            return 1; // b é o maior
    }
    
    return 2; // c é o maior
}



