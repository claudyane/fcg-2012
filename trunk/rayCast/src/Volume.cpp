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
