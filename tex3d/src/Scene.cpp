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


static void mult( const float source[16], float destination[4] )
{
	float result[4];
    
    result[0] = source[0] * destination[0] + source[4] * destination[1] + source[8]  * destination[2] + source[12] * destination[3];
	result[1] = source[1] * destination[0] + source[5] * destination[1] + source[9]  * destination[2] + source[13] * destination[3];
	result[2] = source[2] * destination[0] + source[6] * destination[1] + source[10] * destination[2] + source[14] * destination[3];
	result[3] = source[3] * destination[0] + source[7] * destination[1] + source[11] * destination[2] + source[15] * destination[3];

	for(int i = 0; i < 4; i++)
	{
		destination[i] = result[i];
	}
}



static float algDet( float m[16] ) 
{
  return
    m[0] *
      ( m[5] * (m[10]*m[15] - m[14]*m[11]) - 
        m[9] * (m[6]*m[15]  - m[14]*m[7])  + 
        m[13] * (m[6]*m[11]  - m[10]*m[7])   ) - 
    m[4] *
      ( m[1] * (m[10]*m[15] - m[14]*m[11]) - 
        m[9] * (m[2]*m[15] - m[14]*m[3])   +
        m[13] * (m[2]*m[11] - m[10]*m[3])    ) +
    m[8] * 
      ( m[1] * (m[6]*m[15] - m[14]*m[7]) -
        m[5] * (m[2]*m[15] - m[14]*m[3]) +
        m[13] * (m[2]*m[7] - m[6]*m[3])      ) -
    m[12] *
      ( m[1] * (m[6]*m[11] - m[10]*m[7]) -
        m[5] * (m[2]*m[11] - m[10]*m[3]) +
        m[9] * (m[2]*m[7] - m[6]*m[3])       );
}



static void algInv( float m[16], float inv[16] ) 
{
  double  d;
  //float temp[16];
  d = algDet(m);

  
  inv[0] = ( m[5] * (m[10]*m[15] - m[14]*m[11]) + m[6] * (-m[9]*m[15] + m[13]*m[11]) + 
     m[7] * (m[9]*m[14] - m[13]*m[10]) ) / d;

  inv[4] = - ( m[4] * (m[10]*m[15] - m[14]*m[11]) + m[6] * (-m[8]*m[15] + m[12]*m[11]) + 
       m[7] * (m[8]*m[14] - m[12]*m[10]) ) / d;

  inv[8] = ( m[4] * (m[9]*m[15] - m[13]*m[11]) + m[5] * (-m[8]*m[15] + m[12]*m[11]) + 
     m[7] * (m[8]*m[13] - m[12]*m[9]) ) / d;

  inv[12] = - ( m[4] * (m[9]*m[14] - m[13]*m[10]) + m[5] * (-m[8]*m[14] + m[12]*m[10]) + 
       m[6] * (m[8]*m[13] - m[12]*m[9]) ) / d;



  inv[1] = - ( m[1] * (m[10]*m[15] - m[14]*m[11]) + m[2] * (-m[9]*m[15] + m[13]*m[11]) + 
       m[3] * (m[9]*m[14] - m[13]*m[10]) ) / d;

  inv[5] = ( m[0] * (m[10]*m[15] - m[14]*m[11]) + m[2] * (-m[8]*m[15] + m[12]*m[11]) + 
     m[3] * (m[8]*m[14] - m[12]*m[10]) ) / d;

  inv[9] = - ( m[0] * (m[9]*m[15] - m[13]*m[11]) + m[1] * (-m[8]*m[15] + m[12]*m[11]) + 
       m[3] * (m[8]*m[13] - m[12]*m[9]) ) / d;

  inv[13] = ( m[0] * (m[9]*m[14] - m[13]*m[10]) + m[1] * (-m[8]*m[14] + m[12]*m[10]) + 
     m[2] * (m[8]*m[13] - m[12]*m[9]) ) / d;



  inv[2] = ( m[1] * (m[6]*m[15] - m[14]*m[7]) + m[2] * (-m[5]*m[15] + m[13]*m[7]) + 
     m[3] * (m[5]*m[14] - m[13]*m[6]) ) / d;

  inv[6] = - ( m[0] * (m[6]*m[15] - m[14]*m[7]) + m[2] * (-m[4]*m[15] + m[12]*m[7]) + 
       m[3] * (m[4]*m[14] - m[12]*m[6]) ) / d;

  inv[10] = ( m[0] * (m[5]*m[15] - m[13]*m[7]) + m[1] * (-m[4]*m[15] + m[12]*m[7]) + 
     m[3] * (m[4]*m[13] - m[12]*m[5]) ) / d;

  inv[14] = - ( m[0] * (m[5]*m[14] - m[13]*m[6]) + m[1] * (-m[4]*m[14] + m[12]*m[6]) + 
       m[2] * (m[4]*m[13] - m[12]*m[5]) ) / d;



  inv[3] =  - ( m[1] * (m[6]*m[11] - m[10]*m[7]) + m[2] * (-m[5]*m[11] + m[9]*m[7]) + 
       m[3] * (m[5]*m[10] - m[9]*m[6]) )  / d;

  inv[7] = ( m[0] * (m[6]*m[11] - m[10]*m[7]) + m[2] * (-m[4]*m[11] + m[8]*m[7]) + 
     m[3] * (m[4]*m[10] - m[8]*m[6]) ) / d;

  inv[11] = - ( m[0] * (m[5]*m[11] - m[9]*m[7]) + m[1] * (-m[4]*m[11] + m[8]*m[7]) + 
       m[3] * (m[4]*m[9] - m[8]*m[5]) ) / d;

  inv[15] = ( m[0] * (m[5]*m[10] - m[9]*m[6]) + m[1] * (-m[4]*m[10] + m[8]*m[6]) + 
     m[2] * (m[4]*m[9] - m[8]*m[5]) ) / d;

	//mult( temp, _transformInv );
}



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
    glDisable( GL_CULL_FACE );
    
    // Desenha a caixa envolvente
    drawBox();
    
    // TODO: resolver numero de fatias
    int nx, ny, nz;
    _volume->getNumberOfSamples( nx, ny, nz );
    
    // Define orientação das fatias
    float modelview[16];
    float invModelview[16];
    glGetFloatv( GL_MODELVIEW_MATRIX , modelview );
    algInv( modelview, invModelview );
    
    Vector4D camPos = _camera->getPosition();
    Vector4D center = _camera->getCenter();
    float eye[4] = {camPos.x, camPos.y, camPos.z, camPos.w};
    mult( invModelview, eye );
    
    Vector4D viewDir = Vector4D( center.x - eye[0], center.y - eye[1], center.z - eye[2], 1.0 );
    viewDir.normalize();
    int maxIndex = max( fabs(viewDir.x), fabs(viewDir.y), fabs(viewDir.z) );        
    
    std::cout << "gl: (" << modelview[8]<< ", " << modelview[9] << ", " << modelview[10] << ")\n";
    std::cout << "view: (" << viewDir.x << ", " << viewDir.y << ", " << viewDir.z << ")\n";
    std::cout << "max: " << maxIndex << "\n"; 
    
    if (maxIndex == 0)
    {
        int direction = (modelview[8] > 0 ? 1 : -1 );
        drawXSlices( 2, direction );
    }
    else if (maxIndex == 1)
    {
        int direction = (modelview[9] > 0 ? 1 : -1 );
        drawYSlices( 2, direction );
    }
    else
    {
        int direction = (modelview[10] > 0 ? 1 : -1 );
        drawZSlices( 2, direction );
    }
    
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



void Scene::drawXSlices( int num, int direction  )
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
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //glEnable( GL_TEXTURE_3D );
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    if (direction < 0)
    {
        for( int slice = num; slice >= 0; --slice )
        {
            float xPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( texPos, 0.0f, 0.0f );
            glTexCoord3f( texPos, 0.0f, 0.0f ); glVertex3f( xPos, 0.0f, 0.0f );
            glTexCoord3f( texPos, 0.0f, 1.0f ); glVertex3f( xPos, 0.0f, zMax );
            glTexCoord3f( texPos, 1.0f, 1.0f ); glVertex3f( xPos, yMax, zMax );
            glTexCoord3f( texPos, 1.0f, 0.0f ); glVertex3f( xPos, yMax, 0.0f );
        }
    }
    else
    {
        for( int slice = 0; slice <= num; ++slice )
        {
            float xPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( texPos, 0.0f, 0.0f );
            glTexCoord3f( texPos, 0.0f, 0.0f ); glVertex3f( xPos, 0.0f, 0.0f );
            glTexCoord3f( texPos, 0.0f, 1.0f ); glVertex3f( xPos, 0.0f, zMax );
            glTexCoord3f( texPos, 1.0f, 1.0f ); glVertex3f( xPos, yMax, zMax );
            glTexCoord3f( texPos, 1.0f, 0.0f ); glVertex3f( xPos, yMax, 0.0f );
        }
    }
    
    glEnd();
    
    glPopAttrib();
}



void Scene::drawYSlices( int num, int direction  )
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
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //glEnable( GL_TEXTURE_3D );    
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    if (direction < 0)
    {
        for( int slice = num; slice >= 0; --slice )
        {
            float yPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( 0.0f, texPos, 0.0f );
            glTexCoord3f( 0.0f, texPos, 0.0f ); glVertex3f( 0.0f, yPos, 0.0f );
            glTexCoord3f( 0.0f, texPos, 1.0f ); glVertex3f( 0.0f, yPos, zMax );
            glTexCoord3f( 1.0f, texPos, 1.0f ); glVertex3f( xMax, yPos, zMax );
            glTexCoord3f( 1.0f, texPos, 0.0f ); glVertex3f( xMax, yPos, 0.0f );
        }
    }
    else
    {
       for( int slice = 0; slice <= num; ++slice )
        {
            float yPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( 0.0f, texPos, 0.0f );
            glTexCoord3f( 0.0f, texPos, 0.0f ); glVertex3f( 0.0f, yPos, 0.0f );
            glTexCoord3f( 0.0f, texPos, 1.0f ); glVertex3f( 0.0f, yPos, zMax );
            glTexCoord3f( 1.0f, texPos, 1.0f ); glVertex3f( xMax, yPos, zMax );
            glTexCoord3f( 1.0f, texPos, 0.0f ); glVertex3f( xMax, yPos, 0.0f );
        } 
    }
    
    
    glEnd();
    
    glPopAttrib();
}



void Scene::drawZSlices( int num, int direction  )
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
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //glEnable( GL_TEXTURE_3D );  
    glBindTexture( GL_TEXTURE_3D, _textureID );
    
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    glBegin( GL_QUADS );
    
    if (direction < 0)
    {
        for( int slice = num; slice >= 0; --slice )
        {
            float zPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( 0.0f, 0.0f, texPos );
            glTexCoord3f( 0.0f, 0.0f, texPos ); glVertex3f( 0.0f, 0.0f, zPos );
            glTexCoord3f( 0.0f, 1.0f, texPos ); glVertex3f( 0.0f, yMax, zPos );
            glTexCoord3f( 1.0f, 1.0f, texPos ); glVertex3f( xMax, yMax, zPos );
            glTexCoord3f( 1.0f, 0.0f, texPos ); glVertex3f( xMax, 0.0f, zPos );
        }
    }
    else
    {
        for( int slice = 0; slice <= num; ++slice )
        {
            float zPos = positionStep * slice;
            float texPos = textureStep * slice;
            glColor3f( 0.0f, 0.0f, texPos );
            glTexCoord3f( 0.0f, 0.0f, texPos ); glVertex3f( 0.0f, 0.0f, zPos );
            glTexCoord3f( 0.0f, 1.0f, texPos ); glVertex3f( 0.0f, yMax, zPos );
            glTexCoord3f( 1.0f, 1.0f, texPos ); glVertex3f( xMax, yMax, zPos );
            glTexCoord3f( 1.0f, 0.0f, texPos ); glVertex3f( xMax, 0.0f, zPos );
        }
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
    
    glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
    // x = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, zMax );
    glVertex3f( 0.0f, yMax, zMax );
    glVertex3f( 0.0f, yMax, 0.0f );
    
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    // x = xMax
    glVertex3f( xMax, 0.0f, 0.0f );
    glVertex3f( xMax, yMax, 0.0f );
    glVertex3f( xMax, yMax, zMax );
    glVertex3f( xMax, 0.0f, zMax );
    
    glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
    // y = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( xMax, 0.0f, 0.0f );
    glVertex3f( xMax, 0.0f, zMax );
    glVertex3f( 0.0f, 0.0f, zMax );
    
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    // y = yMax
    glVertex3f( 0.0f, yMax, 0.0f );
    glVertex3f( 0.0f, yMax, zMax );
    glVertex3f( xMax, yMax, zMax );
    glVertex3f( xMax, yMax, 0.0f );
    
    glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
    // z = 0
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, yMax, 0.0f );
    glVertex3f( xMax, yMax, 0.0f );
    glVertex3f( xMax, 0.0f, 0.0f );
    
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
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



