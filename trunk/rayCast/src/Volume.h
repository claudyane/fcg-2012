/* 
 * File:   Volume.h
 * Author: walther
 *
 * Created on May 28, 2012, 5:01 PM
 */

#ifndef VOLUME_H
#define	VOLUME_H

#include <vector>
#include "Color.h"

class Volume
{
public:
    
    Volume( int nx, int ny, int nz, float dx, float dy, float dz, int offset );
    
    virtual ~Volume();
    
    int getTotalNumberOfSamples();
    
    int getNumberOfSamples( int& nx, int& ny, int& nz );
    
    void setVoxel( int i, int j, int k, byte value );
    
private:
    
    void setNumberOfSamples( int nx, int ny, int nz );
    
    void setSpacing( float dx, float dy, float dz );
    
    int _nx, _ny, _nz;
    float _dx, _dy, _dz;
    int _offset;
    unsigned char* _data;
    
    Color _transferFunction[256];
};

#endif	/* VOLUME_H */

