/*
 * Scene.cpp
 *
 *  Created on: May 14, 2012
 *      Author: eliana
 */

#include "Scene.h"
#include "Image.h"
#include "MainWindowPresenter.h"
#include <cfloat>
#include <cstdio>

Scene::Scene()
{
    _camera = NULL;

}



Scene::~Scene()
{
    delete _camera;
    _camera = 0;

    for (int i = 0; i < _objects.size(); i++)
    {
        delete _objects[i];
    }
    _objects.clear();

    for (int i = 0; i < _lights.size(); i++)
    {
        delete _lights[i];
    }
    _lights.clear();

    for (int i = 0; i < _materials.size(); i++)
    {
        delete _materials[i];
    }
    _materials.clear();

}



void Scene::setCamera( Camera* camera )
{
    if (_camera != NULL)
        delete _camera;

    _camera = camera;
}



void Scene::setAmbientLight( Vector4D ambientLight )
{
    _ambientLight = ambientLight;
}



void Scene::setBackgroundColor( Vector4D backgroungColor )
{
    _backgroundColor = backgroungColor;
}



void Scene::addLight( Light* light )
{
    _lights.push_back( light );
}



void Scene::addObject( Object* object )
{
    _objects.push_back( object );
}



Image* Scene::render()
{
    int width, height;
    _camera->getScreenSize( width, height );
    
    Image* image = imgCreate( width, height, 3 );
    
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            Ray ray = _camera->computeRay( x, y );
            float r, g, b;
            computeRayColor( ray, r, g, b );
            imgSetPixel3f( image, x, y, r, g, b );
        }
    }
    
    return image;
}



void Scene::computeRayColor( Ray ray, float& rOut, float& gOut, float& bOut )
{
    Vector4D point;
    Vector4D normal;
    int objectID;
    
    if (!computeNearestRayIntersection( ray, point, normal, objectID ))
    {
        // no interceptions
        rOut = _backgroundColor.x;
        gOut = _backgroundColor.y;
        bOut = _backgroundColor.z;
    }
    else
    {
        // Get shaded color of the objects material
        shade( objectID, normal, point, rOut, gOut, bOut );
    }
}



void Scene::addMaterial( Material* material )
{
    _materials.push_back( material );
}



Material* Scene::getMaterial( int index )
{
    return _materials[index];
}



bool Scene::computeNearestRayIntersection( Ray ray, Vector4D& point, Vector4D& normal, int& objectID )
{
    int nObjects = _objects.size();
    double tMin = DBL_MAX;

    int nearestId;
    
    for (int objID = 0; objID < nObjects; ++objID)
    {
        double t;
        Object* object = _objects[objID];
        bool intersected = object->computeRayIntersection( ray, t );
        
        if (intersected && (t < tMin))
        {
            tMin = t;
            nearestId = objID;
        }
    }
    
    if (tMin < DBL_MAX)
    {
        objectID = nearestId;
        point = ray.origin + tMin*ray.direction;
        normal = _objects[objectID]->getNormal( point );
        return true;
    }
    
    return false;
}



void Scene::shade( int objectID, Vector4D& normal, Vector4D& point, float& rOut, float& gOut, float& bOut )
{
    rOut = 0.0f;
    gOut = 0.0f;
    bOut = 0.0f;
    
    Object* object = _objects[objectID];
    addAmbienteComponent( object->getMaterialId(), rOut, gOut, bOut );
    
    int numLights = _lights.size();
    
    for (int lightID = 0; lightID < numLights; ++lightID)
    {
        if (!inShadow( point, lightID, objectID ))
        {
            addLambertianComponent( object->getMaterialId(), lightID, normal, point, rOut, gOut, bOut );
            addSpecularComponent( object->getMaterialId(), lightID, normal, point, rOut, gOut, bOut );
        }
    }
}

void Scene::addAmbienteComponent(int materialID, float& red, float& green, float& blue)
{
    // recupera cor difusa do material
    float diffuseR, diffuseG, diffuseB;
    _materials[materialID]->getDiffuse( diffuseR, diffuseG, diffuseB );
    
    // iluminação ambiente
    red   += _ambientLight.x * diffuseR;
    green += _ambientLight.y * diffuseG;
    blue  += _ambientLight.z * diffuseB;   
}



bool Scene::inShadow( Vector4D& point, int lightID, int objectID )
{
    // create a ray from the point to the light source
    Ray toLight;
    toLight.origin = point;
    toLight.direction = _lights[lightID]->getPosition() - toLight.origin;
    toLight.direction.normalize();
    
    // calcula o valor de t no ponto onde está a luz
    double tLight;
    if (toLight.direction.x != 0.0)
        tLight = (_lights[lightID]->getPosition().x - toLight.origin.x) / toLight.direction.x;
    else if (toLight.direction.y != 0.0)
        tLight = (_lights[lightID]->getPosition().y - toLight.origin.y) / toLight.direction.y;
    else if (toLight.direction.z != 0.0)
        tLight = (_lights[lightID]->getPosition().z - toLight.origin.z) / toLight.direction.z;
    else
        return false;    
    
    unsigned int nObjects = _objects.size();
    for (unsigned int id = 0; id < nObjects; id++)
    {
        if (id == objectID)
            continue;
        
        double t;
        Object* currentObject = _objects[id];
        
        if (currentObject->computeRayIntersection( toLight, t ) && t > 0 && t < tLight)
        {
            return true;
        }        
    }
    
    return false;
}



void Scene::addLambertianComponent(int materialID, int lightID, Vector4D& normal, Vector4D& point, float& red, float& green, float& blue)
{
    Vector4D lightDir = _lights[lightID]->getPosition() - point;
    lightDir.normalize();
    double cosTheta = dot( normal, lightDir );

    if (cosTheta < 0)
        cosTheta = 0.0;

    float lightR, lightG, lightB;
    _lights[lightID]->getDiffuse( lightR, lightG, lightB );

    float diffuseR, diffuseG, diffuseB;
    _materials[materialID]->getDiffuse( diffuseR, diffuseG, diffuseB );
    
    // iluminação lambertiana
    red   += diffuseR * lightR * cosTheta;
    green += diffuseG * lightG * cosTheta;
    blue  += diffuseB * lightB * cosTheta;
}


void Scene::addSpecularComponent( int materialID, int lightID, Vector4D& normal, Vector4D& point, float& red, float& green, float& blue )
{
    Vector4D lightDir = _lights[lightID]->getPosition() - point;
    lightDir.normalize();
    double cosTheta = dot( normal, lightDir );

    if (cosTheta < 0)
        cosTheta = 0.0;

    float lightR, lightG, lightB;
    _lights[lightID]->getDiffuse( lightR, lightG, lightB );

    // componente especular
    float specularR, specularG, specularB, exponent;
    _materials[materialID]->getSpecular( specularR, specularG, specularB, exponent );

    Vector4D r = ( 2 * cosTheta * normal ) - lightDir;
    r.normalize();

    Vector4D eyeDir = _camera->getPosition() - point;
    eyeDir.normalize();

    float specularCoeficient = pow( dot(r, eyeDir), exponent );
    red   += specularR * lightR * specularCoeficient;
    green += specularG * lightG * specularCoeficient;
    blue  += specularB * lightB * specularCoeficient;
}