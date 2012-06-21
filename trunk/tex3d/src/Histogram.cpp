/* 
 * File:   Histogram.cpp
 * Author: walther
 * 
 * Created on June 21, 2012, 4:55 PM
 */

#include <cstring>
#include <GL/gl.h>

#include "Histogram.h"

Histogram::Histogram( int width, int height )
{
    clear();
    
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
}

void Histogram::draw()
{
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glOrtho( 0, _width, 0, _height, 0, 2 );
    
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    
    // do shit!
    
    glPopAttrib();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
}

void Histogram::clear()
{
    memset( _values, 0, sizeof( _values ) );
}