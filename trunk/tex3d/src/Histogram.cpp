/* 
 * File:   Histogram.cpp
 * Author: walther
 * 
 * Created on June 21, 2012, 4:55 PM
 */

#include <cstring>
#include <string>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <sstream>

#include "Histogram.h"

Histogram::Histogram( int width, int height )
{
    clear();
    
    _volume = 0;
    
    _rightBorder = 5;
    _leftBorder = _rightBorder;
    _topBorder = _rightBorder;
    
    float characterHeight = glutBitmapHeight( GLUT_BITMAP_9_BY_15 );    
    _bottomBorder = _rightBorder + characterHeight;
    
    _width = width;
    _height = height;
}

Histogram::~Histogram()
{
    // do nothing
}

void Histogram::setVolume(Volume* volume)
{    
    clear();
    
    _volume = volume;
    
    if (!volume)
        return;
    
    int nx, ny, nz;
    volume->getNumberOfSamples( nx, ny, nz );
    
    for( int x = 0; x < nx; ++x )
    {
        for( int y = 0; y < ny; ++y )
        {
            for( int z = 0; z < nz; ++z )
            {
                byte value = volume->getVoxel( x, y, z );
                _values[value] += 1;
            }
        }
    }
    
    for( int i = 0; i < 255; ++i )
    {
        if( _maxValue < _values[i] )
        {
            _maxValue = _values[i];
        }
    }
}

void Histogram::draw()
{
    setupOGL();
    
    glClear( GL_COLOR_BUFFER_BIT );
    
    drawHorizontalAxis();
    
    if (_volume)
    {       
        // calcula espaçamento horizontal e vertical das amostras
        float dx = (_width - _rightBorder - _leftBorder)/256.0f; 
        float dy = (_height - _topBorder - _bottomBorder - 1.0f )/_maxValue; 

        glBegin( GL_QUADS );
        for (int i = 0; i < 255; i++)
        {
            int red = i % 2; 

            glColor3f( (float)red, 0.0f, 0.0f );

            int value = _values[i];
            float valueHeight = value*dy + _bottomBorder + 1.0f;
            float valuePos = i*dx;
            glVertex2f( valuePos     , _bottomBorder + 1.0f );
            glVertex2f( valuePos     , valueHeight          );
            glVertex2f( valuePos + dx, valueHeight          );
            glVertex2f( valuePos + dx, _bottomBorder + 1.0f );
        }
        glEnd();
    }
    
    cleanupOGL();
}

void Histogram::drawHorizontalAxis()
{
    float characterHeight = glutBitmapHeight( GLUT_BITMAP_9_BY_15 );
    glColor3f( 0.0f, 0.0f, 0.0f );
    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    glVertex2f( _leftBorder, _bottomBorder + 1.0f );
    glVertex2f( _width-_rightBorder, _bottomBorder + 1.0f );
    glEnd();
    
    glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
    float dx = (_width - _rightBorder - _leftBorder)/255.0f;    
    for (int i = 0; i < 256; i+= 16)
    {
        std::stringstream text;
        text << i;        
        glRasterPos2f( _leftBorder + i*dx, _bottomBorder - characterHeight);
        glutBitmapString( GLUT_BITMAP_9_BY_15, (const unsigned char*) text.str().c_str() );   
    }       
}

void Histogram::setupOGL()
{
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, _width, 0, _height );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
}

void Histogram::cleanupOGL()
{
    glPopAttrib();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}

void Histogram::clear()
{
    _maxValue = 0;
    memset( _values, 0, sizeof( _values ) );
}



std::string Histogram::getInfo( int x )
{
    float dx = (_width - _rightBorder - _leftBorder)/256.0f; 
    
    int pos = (x-_leftBorder)/dx;
    
    if (!_volume || pos < 0 || pos > 255)
        return "\n";
    
    std::stringstream info;
    info << "Voxel value: " << pos << "\n";
    info << "Number of samples: " << _values[pos] << "\n";
    
    return info.str();
}

