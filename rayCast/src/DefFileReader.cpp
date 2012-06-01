/* 
 * File:   DefFileReader.cpp
 * Author: eliana
 * 
 * Created on May 28, 2012, 5:11 PM
 */

#include "DefFileReader.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

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

    size_t pos = filePath.find_last_of( "/\\" );          
    std::string dataPath = filePath.substr( 0, pos + 1 );
    dataPath += data_file;    
    loadVolumeData( dataPath, volume, offset );

    std::string transferPath = filePath.substr( 0, pos + 1 );
    transferPath += transf_file;
    loadVolumeTransfer( transferPath, volume );
    
    return volume;
}

void DefFileReader::loadVolumeData( std::string dataFilePath, Volume* volume, int offset )
{
    FILE *fp;

    fp = fopen(dataFilePath.c_str(), "rb");
    if (!fp) return;
    
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

Camera* DefFileReader::loadCamera( std::string filePath )
{
    float x, y, z;
    int n;
    char param_name[16];
    bool errors = false;
    FILE* fp = fopen(filePath.c_str(), "r");
    if (!fp)
    return 0;

    /* Lendo a posicao do observador */
    Vector4D eye;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "eye=") == 0)
    {
        fscanf(fp, "%f %f %f", &x, &y, &z);
        eye.x = x;
        eye.y = y;
        eye.z = z;
        eye.w = 1.0;
    }
    else errors = true;
    
    /* Lendo a direcao de visao */
    Vector4D at;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "at=") == 0)
    {
        fscanf(fp, "%f %f %f", &x, &y, &z);
        at.x = x;
        at.y = y;
        at.z = z;
        at.w = 1.0;
    }
    else errors = true;
    
    /* Lendo o "view-up" */
    Vector4D up;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "up=") == 0)
    {
        fscanf(fp, "%f %f %f", &x, &y, &z);
        up.x = x;
        up.y = y;
        up.z = z;
        up.w = 1.0;
    }
    else errors = true;
    
    /* Lendo o fovy */
    float fovy;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "fovy=") == 0)
    {
        fscanf(fp, "%f ", &x);
        fovy = x;
    }
    else errors = true;
    
    /* Lendo o near plane */
    float near;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "nearp=") == 0)
    {
        fscanf(fp, "%f ", &x);
        near = x;
    }
    else errors = true;  
    
    /* Lendo o far plane */
    float far;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "farp=") == 0)
    {
        fscanf(fp, "%f ", &x);
        far = x;
    }
    else errors = true;
    
    /* Lendo a largura da janela */
    int width;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "screenW=") == 0)
    {
        fscanf(fp, "%d ", &n);
        width = n;
    }
    else errors = true;
    
    /* Lendo a altura da janela */
    int height;
    fscanf(fp, "%s ", param_name);
    if (strcmp(param_name, "screenH=") == 0)
    {
        fscanf(fp, "%d ", &n);
        height = n;
    }
    else errors = true;
    
    fclose(fp);
    
    if( errors )
    {
        printf("Ocorreram erros ao ler o arquivo .cam\n");
        exit(1);
    }
    
    return new Camera( eye, at, up, fovy, near, width, height );
}