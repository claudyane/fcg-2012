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
            Vector4D point, normal;
            int objectId;
            
            if (computeNearestRayIntersection( ray, point, normal, objectId))
            {
                float r, g, b, a;       
                Object* selectedObject = _objects[objectId];
                selectedObject->getColor( r, g, b, a );
                imgSetPixel3f( image, x, y, r, g, b );
            }
            else
            {
                imgSetPixel3f( image, x, y, 0.0f, 0.0f, 0.0f );
            }            
        }
    }
    
    return image;
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
    Vector4D nearestNormal;
    int nearestId;
    
    for (int objID = 0; objID < nObjects; ++objID)
    {
        double t;
        Vector4D currentNormal;
        Object* object = _objects[objID];
        bool intersected = object->computeRayIntersection( ray, t, currentNormal );
        
        if (intersected && (t < tMin))
        {
            tMin = t;
            nearestId = objID;
            nearestNormal = currentNormal;
        }
    }
    
    if (tMin < DBL_MAX)
    {
        normal = nearestNormal;
        objectID = nearestId;
        point = ray.origin + tMin*ray.direction;
        
        return true;
    }
    
    return false;
}
