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
#include "Box.h"

typedef unsigned char byte;

class Volume : public Box
{
public:
    
    Volume( int nx, int ny, int nz, float dx, float dy, float dz, int offset );
    
    virtual ~Volume();
    
    int getTotalNumberOfSamples();
    
    int getNumberOfSamples( int& nx, int& ny, int& nz );
    
    void setVoxel( int i, int j, int k, byte value );
    
    byte getVoxel( int i, int j, int k );
    
    void setTransferFunctionPoint( int point, Color value );
    
    void interpolateTransferFunction();
    
    float getSmallestDimension();
    
    Color interpolate( Vector4D point );
    
    Vector4D getCenter();
    
private:
    
    int _nx, _ny, _nz;
    float _dx, _dy, _dz;
    int _offset;
    unsigned char* _data;
    
    Color _transferFunction[256];
    std::vector<int> _fixedTransferPositions;
};

#endif	/* VOLUME_H */

