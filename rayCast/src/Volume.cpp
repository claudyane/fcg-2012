/* 
 * File:   Volume.cpp
 * Author: walther
 * 
 * Created on May 28, 2012, 5:01 PM
 */

#include "Volume.h"

Volume::Volume(int nx, int ny, int nz, float dx, float dy, float dz, int offset)
{
}

Volume::~Volume()
{
}

int Volume::getNumberOfSamples(int& nx, int& ny, int& nz){}

int Volume::getTotalNumberOfSamples(){}

void Volume::interpolateTransferFunction(){}

void Volume::setTransferFunctionPoint(int point, Color value){}

void Volume::setVoxel(int i, int j, int k, byte value){}

void Volume::setNumberOfSamples(int nx, int ny, int nz){}

void Volume::setSpacing(float dx, float dy, float dz){}
