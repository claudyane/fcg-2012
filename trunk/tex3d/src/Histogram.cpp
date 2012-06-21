/* 
 * File:   Histogram.cpp
 * Author: walther
 * 
 * Created on June 21, 2012, 4:55 PM
 */

#include <cstring>

#include "Histogram.h"

Histogram::Histogram( int width, int height )
{
    clear();
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
}

void Histogram::clear()
{
    memset( _values, 0, sizeof( _values ) );
}