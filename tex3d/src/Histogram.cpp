/* 
 * File:   Histogram.cpp
 * Author: walther
 * 
 * Created on June 21, 2012, 4:55 PM
 */

#include <cstring>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Histogram.h"

Histogram::Histogram( int width, int height )
{
    clear();
    
    _rightBorder = 0.05 * width;
    _topBorder = _rightBorder;
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
    
    cleanupOGL();
}

void Histogram::drawHorizontalAxis()
{
    float characterHeight = glutBitmapHeight( GLUT_BITMAP_9_BY_15 );
    glColor3f( 0.0f, 0.0f, 0.0f );
    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    glVertex2f( 0.0f, characterHeight + 1.0f );
    glVertex2f( _width, characterHeight + 1.0f );
    glEnd();
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