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

Volume::Volume(int nx, int ny, int nz, float dx, float dy, float dz, int offset)
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
    _data[index(i, j, k)] = value;
}



byte Volume::getVoxel( int i, int j, int k )
{
    return _data[index(i, j, k)];
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
    
    x -= i1;
    y -= j1;
    z -= k1;
    
//    x = fabs(point.x - i1*_dx)/_dx;
//    y = fabs(point.y - j1*_dy)/_dy;
//    z = fabs(point.z - k1*_dz)/_dz;
    
    if( i1 <= 0 || i2 >= _nx || j1 <= 0 || j2 >= _ny || k1 <= 0 || k2 >= _nz )
    {
        return Color( 0.0f, 0.0f, 0.0f, 0.0f );
    }
    
    Color colorOut( 0.0f, 0.0f, 0.0f, 0.0f );
    
    // (i, j, k)
    byte v1 = getVoxel( i1, j1, k1 );
    //Color colorV1 = _transferFunction[v1];    
    Color colorV1 = weightOpacity( _transferFunction[v1] );
    colorOut += (1-x)*(1-y)*(1-z)*colorV1;
    
    // (i+1, j, k)
    byte v2 = getVoxel( i2, j1, k1 );
    //Color colorV2 = _transferFunction[v2];
    Color colorV2 = weightOpacity( _transferFunction[v2] );
    colorOut += x*(1-y)*(1-z)*colorV2;
    
    // (i, j+1, k)
    byte v3 = getVoxel( i1, j2, k1 );
    //Color colorV3 = _transferFunction[v3];
    Color colorV3 = weightOpacity( _transferFunction[v3] );
    colorOut += (1-x)*y*(1-z)*colorV3;
    
    // (i+1, j+1, k)
    byte v4 = getVoxel( i2, j2, k1 );
    //Color colorV4 = _transferFunction[v4];
    Color colorV4 = weightOpacity( _transferFunction[v4] );
    colorOut += x*y*(1-z)*colorV4;
    
    // (i, j, k+1)
    byte v5 = getVoxel( i1, j1, k2 );
    //Color colorV5 = _transferFunction[v5];
    Color colorV5 = weightOpacity( _transferFunction[v5] );
    colorOut += (1-x)*(1-y)*z*colorV5;
    
    // (i+1, j, k+1)
    byte v6 = getVoxel( i2, j1, k2 );
    //Color colorV6 = _transferFunction[v6];
    Color colorV6 = weightOpacity( _transferFunction[v6] );
    colorOut += x*(1-y)*z*colorV6;
    
    // (i, j+1, k+1)
    byte v7 = getVoxel( i1, j2, k2 );
    //Color colorV7 = _transferFunction[v7];
    Color colorV7 = weightOpacity( _transferFunction[v7] );
    colorOut += (1-x)*y*z*colorV7;
    
    // (i, j+1, k+1)
    byte v8 = getVoxel( i2, j2, k2 );
    //Color colorV8 = _transferFunction[v8];
    Color colorV8 = weightOpacity( _transferFunction[v8] );
    colorOut += x*y*z*colorV8;
    
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
    
    if( i >= _nx || i < 0 || j >= _ny || j < 0 || k >= _nz || k < 0 )
        return Vector4D( 0.0, 0.0, 0.0, 1.0 );
        
    int prevI = (i > 0? i-1 : 0);
    int prevJ = (j > 0? j-1 : 0);
    int prevK = (k > 0? k-1 : 0);
    
    int nextI = (i < _nx-1? i+1 : _nx-1);
    int nextJ = (j < _ny-1? j+1 : _ny-1);
    int nextK = (k < _nz-1? k+1 : _nz-1);
    
    Vector4D normal;
    
    normal.x = (getVoxel( nextI, j, k ) - getVoxel( prevI, j, k ))/(2*_dx);
    normal.y = (getVoxel( i, nextJ, k ) - getVoxel( i, prevJ, k ))/(2*_dy);
    normal.z = (getVoxel( i, j, nextK ) - getVoxel( i, j, prevK ))/(2*_dz);
    normal.w = 1.0;
    
    Color nowColor  = _transferFunction[getVoxel( i, j, k )];
    
    Color nextColor = _transferFunction[getVoxel( nextI, j, k )];
    if( nowColor.r == nextColor.r && nowColor.g == nextColor.g && nowColor.b == nextColor.b && nowColor.a == nextColor.a )
    {
        normal.x = 0;
    }
    
    nextColor = _transferFunction[getVoxel( i, nextJ, k )];
    if( nowColor.r == nextColor.r && nowColor.g == nextColor.g && nowColor.b == nextColor.b && nowColor.a == nextColor.a )
    {
        normal.y = 0;
    }
    
    nextColor = _transferFunction[getVoxel( i, j, nextK )];
    if( nowColor.r == nextColor.r && nowColor.g == nextColor.g && nowColor.b == nextColor.b && nowColor.a == nextColor.a )
    {
        normal.z = 0;
    }
    //normal.normalize();
    
    return normal;
}



float* Volume::getTexture3D()
{
    float* texture = new float[_nx*_ny*_nz];
    
    for (int i = 0; i < _nx; i++)
    {
        for (int j = 0; j < _ny; j++)
        {
            for (int k = 0; k < _nz; k++)
            {
                int sampleIndex = index( i, j, k );
                Color voxelColor = _transferFunction[getVoxel( i, j, k )];
                
                texture[4*sampleIndex  ] = voxelColor.r;
                texture[4*sampleIndex+1] = voxelColor.g;
                texture[4*sampleIndex+2] = voxelColor.b;
                texture[4*sampleIndex+3] = voxelColor.a;
            }
        }
    }
    
    return texture;
}



int Volume::index( int i, int j, int k )
{
    return i*_ny*_nz + j*_nz + k;
}

