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
#include <cstdio>
#include <iostream>

Volume::Volume(int nx, int ny, int nz, float dx, float dy, float dz, int offset) : 
        Box( Vector4D(0.0, 0.0, 0.0, 1.0), Vector4D(nx*dx, ny*dy, nz*dz, 1.0) )
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
    
    for(unsigned int i = 0; i < nFixedPositions; ++i )
    {
        int tEnd = _fixedTransferPositions[i];
        int dt = tEnd - tBegin;
        Color increment = (_transferFunction[tEnd] - _transferFunction[tBegin]);
        increment /= dt;
        
        Color sampleValue =_transferFunction[tBegin] + increment;
        
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
    _data[i*_ny*_nz + j*_nz + k] = value;
}



byte Volume::getVoxel( int i, int j, int k )
{
    return _data[i*_ny*_nz + j*_nz + k];
}



float Volume::getSmallestDimension()
{
    float minxy = ( _dx < _dy ? _dx : _dy );
    return ( minxy < _dz ? minxy : _dz );
}



Color Volume::interpolate( Vector4D point )
{
    double x = ( point.x / _dx );
    double y = ( point.y / _dy );
    double z = ( point.z / _dz );
    
    int i1 = (int) x;
    int j1 = (int) y;
    int k1 = (int) z;
    int i2 = i1+1;
    int j2 = j1+1;
    int k2 = k1+1;
    
    if( i1 <= 0 || i2 >= _nx || j1 <= 0 || j2 >= _ny || k1 <= 0 || k2 >= _nz )
    {
        return Color( 0.0f, 0.0f, 0.0f, 0.0f );
    }
    
    // i1,j1,k1 -- i2,j1,k1
    double V1 = ( i2-x )*getVoxel( i1, j1, k1 ) + ( x-i1 )*getVoxel( i2, j1, k1 );
    
    // i1,j2,k1 -- i2,j2,k1
    double V2 = ( i2-x )*getVoxel( i1, j2, k1 ) + ( x-i1 )*getVoxel( i2, j2, k1 );
    
    // i1,j1,k2 -- i2,j1,k2
    double V3 = ( i2-x )*getVoxel( i1, j1, k2 ) + ( x-i1 )*getVoxel( i2, j1, k2 );
    
    // i1,j2,k2 -- i2,j2,k2
    double V4 = ( i2-x )*getVoxel( i1, j2, k2 ) + ( x-i1 )*getVoxel( i2, j2, k2 );
    
    // V1 -- V2
    double V5 = ( j2-y )*V1 + ( y-j1 )*V2;
    
    // V3 -- V4
    double V6 = ( j2-y )*V3 + ( y-j1 )*V4;
    
    // V5 -- V6
    double V7 = ( k2-z )*V5 + ( z-k1 )*V6;
    
    int value = (int)(V7+0.5);
    
    Color colorOut = _transferFunction[ value ];
    
//    if( value - (int)getVoxel( i1, j1, k1 ) != 0 )
//    {
//        std::cout << "interpolated: " << value << "\n";
//        std::cout << "sharp: " << (int)getVoxel( i1, j1, k1 ) << "\n\n";
//    }
    
    
    return colorOut;
}



Vector4D Volume::getCenter()
{
    return Vector4D( (_nx*_dx)/2.0, (_ny*_dy)/2.0, (_nz*_dz)/2.0, 1.0 );
}



Vector4D Volume::getNormal( Vector4D& point )
{
    int i = (int)( point.x / _dx );
    int j = (int)( point.y / _dy );
    int k = (int)( point.z / _dz );
    
    int prevI = (i > 0? i-1 : 0);
    int prevJ = (j > 0? j-1 : 0);
    int prevK = (k > 0? k-1 : 0);
    
    int nextI = (i < _nx-1? i+1 : _nx-1);
    int nextJ = (j < _ny-1? i+1 : _ny-1);
    int nextK = (k < _nz-1? i+1 : _nz-1);
    
    Vector4D normal;
    
    normal.x = (getVoxel( nextI, j, k ) - getVoxel( prevI, j, k ))/(2*_dx);
    normal.y = (getVoxel( i, nextJ, k ) - getVoxel( i, prevJ, k ))/(2*_dy);
    normal.z = (getVoxel( i, j, nextK ) - getVoxel( i, j, prevK ))/(2*_dz);
    normal.w = 1.0;
    
    normal.normalize();
    
    return normal;
}

