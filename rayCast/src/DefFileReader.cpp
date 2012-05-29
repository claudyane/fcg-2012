/* 
 * File:   DefFileReader.cpp
 * Author: eliana
 * 
 * Created on May 28, 2012, 5:11 PM
 */

#include "DefFileReader.h"

#include <cstdio>

typedef unsigned char byte;

DefFileReader::DefFileReader()
{
}

DefFileReader::~DefFileReader()
{
}


Volume* DefFileReader::loadVolume( std::string filePath )
{
    FILE* fp;
    int nx, ny, nz, offset;
    float dx, dy, dz;
    char data_file[256];
    char transf_file[256];

    fp = fopen(filePath.c_str(), "r");
    if (!fp) return 0;
    
    fscanf(fp, "%d %d %d %f %f %f %d\n", &nx, &ny, &nz, &dx, &dy, &dz, &offset);
    fscanf(fp, "%s\n", data_file);
    fscanf(fp, "%s\n", transf_file);

    fclose(fp);
    
    Volume* volume = new Volume( nx, ny, nz, dx, dy, dz, offset );

    loadVolumeData( data_file, volume, offset );

    loadVolumeTransfer( transf_file, volume );
    
    return volume;
}

void DefFileReader::loadVolumeData( std::string dataFilePath, Volume* volume, int offset )
{
    FILE *fp;

    fp = fopen(dataFilePath.c_str(), "rb");
    if (!fp) return;
    
    /* Alocando o buffer de dados volumetricos */
    //byte* data = new byte[volume->getTotalNumberOfSamples()];
    
    /* Passando o header */
    fseek(fp, offset, 0);
    
    /* Lendo os dados */
    int nx, ny, nz;
    volume->getNumberOfSamples( nx, ny, nz );
    
    for(int k=0; k < nz; k++)
    {
        for(int j=0; j < ny; j++)
        {
            for(int i=0; i < nx; i++)
            {
                volume->setVoxel( i, j, k, (byte)getc( fp ) );
            }
        }
    }
    
    fclose(fp);
}

void DefFileReader::loadVolumeTransfer( std::string transferFilePath, Volume* volume )
{
    // open the file
    FILE* fp = fopen( transferFilePath.c_str(), "r" );
    if (!fp) return;
    
    // read number of samples from file
    int numberOfSamples = 0;
    fscanf( fp, "%d\n", &numberOfSamples );
    if (numberOfSamples < 2) return;
    
    for (int sample = 0; sample < numberOfSamples; ++sample)
    {
        // read sample line from file
        float r1, g1, b1, a1;
        int t1;
        fscanf( fp, "%d %f %f %f %f\n", &t1, &r1, &g1, &b1, &a1 );
        
        // set the sample value on the volume
        Color value( r1, g1, b1, a1 );
        volume->setTransferFunctionPoint( t1, value );
    }
    
    // close file
    fclose(fp);
    
    // interpolate the intermediate values on the transfer function
    volume->interpolateTransferFunction();
}
