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
    int objectId;
    
    if (!computeNearestRayIntersection( ray, point, normal, objectId ))
    {
        // no interceptions
        rOut = _backgroundColor.x;
        gOut = _backgroundColor.y;
        bOut = _backgroundColor.z;
    }
    else
    {
        // Get shaded color of the objects material
        Object* selectedObject = _objects[objectId];
        shade( selectedObject->getMaterialId(), normal, point, rOut, gOut, bOut );
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



void Scene::shade( int materialId, Vector4D& normal, Vector4D& point, float& rOut, float& gOut, float& bOut )
{
    // recupera cor difusa do material
    float diffuseR, diffuseG, diffuseB;
    _materials[materialId]->getDiffuse( diffuseR, diffuseG, diffuseB );
    
    // iluminação ambiente
    rOut = _ambientLight.x * diffuseR;
    gOut = _ambientLight.y * diffuseG;
    bOut = _ambientLight.z * diffuseB;   
    
    int numLights = _lights.size();
    
    for (int lightID = 0; lightID < numLights; ++lightID)
    {
        Vector4D lightDir = _lights[lightID]->getPosition() - point;
        lightDir.normalize();
        double cosTheta = dot( normal, lightDir );
        
        if (cosTheta < 0)
            cosTheta = 0.0;
        
        float lightR, lightG, lightB;
        _lights[lightID]->getDiffuse( lightR, lightG, lightB );
        
        // iluminação lambertiana
        rOut += diffuseR * lightR * cosTheta;
        gOut += diffuseG * lightG * cosTheta;
        bOut += diffuseB * lightB * cosTheta;
        
        // componente especular
        float specularR, specularG, specularB, exponent;
        _materials[materialId]->getSpecular( specularR, specularG, specularB, exponent );
        
        Vector4D r = ( 2 * cosTheta * normal ) - lightDir;
        r.normalize();
        
        Vector4D eyeDir = _camera->getPosition() - point;
        eyeDir.normalize();

        float specularCoeficient = pow( dot(r, eyeDir), exponent );
        rOut += specularR * lightR * specularCoeficient;
        gOut += specularG * lightG * specularCoeficient;
        bOut += specularB * lightB * specularCoeficient;
    }
}
