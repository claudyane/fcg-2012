/*
 * Scene.cpp
 *
 *  Created on: May 14, 2012
 *      Author: eliana
 */

#include "Scene.h"
#include "Image.h"

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
    int numObjects = _objects.size();
    
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            Ray ray = _camera->computeRay( x, y );
            
            for (int objectId = 0; objectId < numObjects; ++objectId)
            {
                Vector4D point;
                Vector4D normal;
                if (_objects[objectId]->computeRayIntersection( ray, point, normal ))
                {
                    float r, g, b, a;
                    _objects[objectId]->getColor( r, g, b, a );
                    imgSetPixel3f( image, x, y, r, g, b );
                }
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
    return _materials[0];
}
