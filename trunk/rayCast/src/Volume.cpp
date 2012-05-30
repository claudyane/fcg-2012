/* 
 * File:   Volume.cpp
 * Author: walther
 * 
 * Created on May 28, 2012, 5:01 PM
 */

#include <math.h>
#include <algorithm>

#include "Volume.h"
#include "Vector4D.h"

Volume::Volume(int nx, int ny, int nz, float dx, float dy, float dz, int offset) : 
        Box( Vector4D(0.0, 0.0, 0.0, 1.0), Vector4D(nx, ny, nz, 1.0) )
{
    _nx = nx;
    _ny = ny;
    _nz = nz;
    
    _dx = dx;
    _dy = dy;
    _dz = dz;
    
    _offset = offset;
    
    _data = new byte[nx * ny * nz];
}

Volume::~Volume()
{
    delete[] _data;
}

int Volume::getNumberOfSamples(int& nx, int& ny, int& nz)
{
    nx = _nx;
    ny = _ny;
    nz = _nz;
}

int Volume::getTotalNumberOfSamples()
{
    return _nx * _ny * _nz;
}

void Volume::interpolateTransferFunction()
{
    // sort
    std::sort( _fixedTransferPositions.begin(), _fixedTransferPositions.end() );
    
    // interpolate
    unsigned int tBegin = _fixedTransferPositions[0];
    unsigned int nFixedPositions = _fixedTransferPositions.size();
    
    for( unsigned int tEnd = _fixedTransferPositions[1]; tEnd < nFixedPositions; ++tEnd )
    {
        int dt = tEnd - tBegin;
        Color increment = (_transferFunction[tEnd] - _transferFunction[tBegin]);
        increment /= dt;
        
        Color sampleValue = increment;
        
        for( int tSample = tBegin+1; tSample < tEnd; ++tSample )
        {
            _transferFunction[tSample] = sampleValue;
            sampleValue += increment;
        }
        
        tBegin = tEnd;
    }
}

void Volume::setTransferFunctionPoint(int point, Color value)
{
    _transferFunction[point] = value;
    _fixedTransferPositions.push_back( point );
}

void Volume::setVoxel(int i, int j, int k, byte value)
{
    _data[i + j * _nx + k * _nx * _ny] = value;
}


byte Volume::getVoxel( int i, int j, int k )
{
    return _data[i + j * _nx + k * _nx * _ny];
}

float Volume::getSmallestDimension()
{
    int minxy = ( _nx < _ny ? _nx : _ny );
    return ( minxy < _nz ? minxy : _nz );
}

Color Volume::interpolate( Vector4D point )
{
    int i = (int)( point.x / _dx );
    int j = (int)( point.y / _dy );
    int k = (int)( point.z / _dz );
    
    Color outValue;
    
    for (int auxi = 0; i <= i+1; ++i, ++auxi)
    {
        for (int auxj = 0; j <= j+1; ++j, ++auxj)
        {
            for (int auxk = 0; k <= k+1; ++k, ++auxk)
            {
                if (i < 0 || i >= _nx || j < 0 || j >= _ny || k < 0 || k >= _nz)
                    continue;
                
                byte voxel = getVoxel( i, j, k );
                
                outValue += ( 1 - point.x + auxi ) *
                            ( 1 - point.y + auxj ) * 
                            ( 1 - point.z + auxk ) * 
                            _transferFunction[voxel];
            }
        }
    }
    
    return outValue;
}
