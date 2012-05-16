/*
 * Rt4FileReader.cpp
 *
 *  Created on: May 14, 2012
 *      Author: eliana
 */

#include "Rt4FileReader.h"
#include "Vector4D.h"
#include <cstdio>
#include "Camera.h"
#include "Sphere.h"
#include "Light.h"

Rt4FileReader::Rt4FileReader()
{
    // TODO Auto-generated constructor stub

}



Rt4FileReader::~Rt4FileReader()
{
    // TODO Auto-generated destructor stub
}



void Rt4FileReader::loadScene( const std::string filename, Scene* scene )
{
    FILE* file;
    char buffer[512];

    float version;
    float aux;
    Vector4D eye, center, up;
    double fovy;
    double near;
    double far;
    int width;
    int height;

    Vector4D backgroundColor;
    Vector4D ambientLight;
    char backgroundFilename[1024];


    Vector4D diffuse;
    Vector4D specular;
    double specularExponent;
    float reflective, refractive, opacity;
    char textureFilename[1024];

    Vector4D lightPosition;
    Vector4D lightDiffuse;

    double radius;
    Vector4D position1, position2, position3;
    Vector4D texture1, texture2, texture3;
    int material;

    // Abre o arquivo para leitura
    file = fopen( filename.c_str(), "r" );
    if( !file )
        return;

    while (fgets( buffer, sizeof(buffer), file ))
    {
        if( sscanf( buffer, "RT %f\n", &version ) == 1 )
        {
             //Ignore File Version Information
        }
        else if(sscanf(buffer, "SOFTSHADOW %f %f", &aux, &aux) == 2)
        {
           // TODO:
        }
        else if( sscanf( buffer, "CAMERA %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d %d\n",
                &eye.x, &eye.y, &eye.z, &center.x, &center.y, &center.z, &up.x, &up.y, &up.z, &fovy, &near, &far,
                &width, &height ) == 14)
        {
            Camera* camera = new Camera( eye, center, up, fovy, near, width, height );
            scene->setCamera( camera );
        }
        else if( sscanf( buffer, "SCENE %lf %lf %lf %lf %lf %lf %s\n", &backgroundColor.x, &backgroundColor.y,
                &backgroundColor.z, &ambientLight.x, &ambientLight.y, &ambientLight.z, backgroundFilename ) == 7 )
        {
            scene->setAmbientLight( ambientLight );
            scene->setBackgroundColor( backgroundColor );
        }
        else if( sscanf( buffer, "MATERIAL %lf %lf %lf %lf %lf %lf %lf %f %f %f %s\n", &diffuse.x, &diffuse.y, &diffuse.z,
                &specular.x, &specular.y, &specular.z, &specularExponent, &reflective, &refractive, &opacity, textureFilename ) == 11 )
        {

        }
        else if( sscanf( buffer, "LIGHT %lf %lf %lf %lf %lf %lf\n", &lightPosition.x, &lightPosition.y, &lightPosition.z,
                &lightDiffuse.x, &lightDiffuse.y, &lightDiffuse.z ) == 6 )
        {
            Light* light = new Light( lightPosition, lightDiffuse );
            scene->addLight( light );
        }
        else if( sscanf( buffer, "SPHERE %d %lf %lf %lf %lf\n", &material, &radius, &position1.x,&position1.y,&position1.z ) == 5 )
        {
            Sphere* sphere = new Sphere( position1.x, position1.y, position1.z, radius );
            // TODO: tratar material
        }
        else if( sscanf( buffer, "TRIANGLE %d %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", &material,
                &position1.x, &position1.y, &position1.z, &position2.x, &position2.y, &position2.z, &position3.x, &position3.y,
                &position3.z, &texture1.x, &texture1.y, &texture2.x, &texture2.y, &texture3.x, &texture3.y) == 16 )
        {
        }
        else if( sscanf( buffer, "BOX %d %lf %lf %lf %lf %lf %lf\n", &material, &position1.x, &position1.y, &position1.z,
                &position2.x, &position2.y, &position2.z ) == 7 )
        {
        }
        //else if( sscanf( buffer, "MESH %d %lf %lf %lf %lf %lf %lf %s\n", &material, &pos1.x, &pos1.y, &pos1.z, &pos2.x, &pos2.y, &pos2.z, buffer ) == 8 )
        //{
        //}
        else
        {
            printf( "sceLoad: Ignorando comando:\n %s\n", buffer );
        }
    }

    //Adjust background image to screen size
    // TODO:

    fclose( file );
}

