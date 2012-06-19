/* 
 * File:   DefFileReader.h
 * Author: eliana
 *
 * Created on May 28, 2012, 5:11 PM
 */

#ifndef DEFFILEREADER_H
#define	DEFFILEREADER_H

#include "Volume.h"
#include "Camera.h"
#include <string>

class DefFileReader
{
public:
    
    DefFileReader();
    
    virtual ~DefFileReader();
    
    Volume* loadVolume( std::string filePath );
    
    Camera* loadCamera( std::string filePath );
    
private:
    
    void loadVolumeData( std::string dataFilePath, Volume* volume, int offset );
    
    void loadVolumeTransfer( std::string transferFilePath, Volume* volume );
};

#endif	/* DEFFILEREADER_H */

