/* 
 * File:   DefFileReader.cpp
 * Author: eliana
 * 
 * Created on May 28, 2012, 5:11 PM
 */

#include "DefFileReader.h"
#include "Volume.h"

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
    
    Volume* volume = new Volume( nx, ny, nz, dx, dy, dz );

    loadVolumeData( data_file, volume, offset );

    loadVolumeTransfer( transf_file, volume );
    
    return volume;
}

void DefFileReader::loadVolumeData( std::string dataFilePath, Volume* volume, int offset )
{
    FILE *fp;

    fp = fopen(dataFilePath.c_str(), "rb");
    if (!fp) return 0;
    
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
    int num;              /* Numero de amostras na curva de transferencia */
    float r1, g1, b1, a1; /* Valores de cor e opacidade da amostra */
    float r2, g2, b2, a2; /* Valores de cor e opacidade da amostra */
    float r, g, b, a;     /* Valores de cor e opacidade INTERPOLADOS */
    float mr, mg, mb, ma; /* Coeficientes de interpolacao */
    float t1, t2, dt;     /* Parametro amostrado */
    FILE* fp;
    int i, j, j1, j2;

    fp = fopen( filename, "r" );
    if (!fp) return 0;
    
    /* Lendo o n�mero de amostras */
    fscanf( fp, "%d\n", &num );
    if (num < 2) return 0;
    
    /* Lendo a primeira amostra */
    fscanf( fp, "%f %f %f %f %f\n", &t1, &r1, &g1, &b1, &a1 );
    j1 = (int)t1;
    
    /* Armazenando a cor da amostra */
    volume->color[j1].x[0] = r1;
    volume->color[j1].x[1] = g1;
    volume->color[j1].x[2] = b1;
    
    /* Armazenando a opacidade da amostra */
    volume->opac[j1] = a1;  
    
    /* Lendo as amostras restantes */
    for(i=1; i < num; i++)
    {
        fscanf(fp, "%f %f %f %f %f\n", &t2, &r2, &g2, &b2, &a2);
        j2 = (int)t2;
        
        /* Armazenando a cor da amostra */
        volume->color[j2].x[0] = r2;
        volume->color[j2].x[1] = g2;
        volume->color[j2].x[2] = b2;
        
        /* Armazenando a opacidade da amostra */
        volume->opac[j2] = a2;  
        
        /* Interpolando entre t1 e t2 */
        dt = t2 - t1;
        mr = (r2 - r1) / dt;
        mg = (g2 - g1) / dt;
        mb = (b2 - b1) / dt;
        ma = (a2 - a1) / dt;
        r = r1; g = g1; b = b1; a = a1;
        for(j=j1+1; j < j2; j++)
        {
            /* Armazenando a cor da amostra */
            volume->color[j].x[0] = r; r += mr;
            volume->color[j].x[1] = (g += mg);
            volume->color[j].x[2] = (b += mb);
            
            /* Armazenando a opacidade da amostra */
            volume->opac[j] = (a += ma);
        }
        
        j1 = j2;
        t1 = t2;
        r1 = r2;
        g1 = g2;
        b1 = b2;
        a1 = a2;
    }
    
    fclose(fp);
    return 1;
}
