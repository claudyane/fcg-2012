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

#define MAX_DEPTH 6

Scene::Scene()
{
    _camera = NULL;
    
    _ambient    = true;
    _diffuse    = true;
    _specular   = true;
    _shadow     = true;
    _reflection = true;
    _softShadow = true;
    
    _softShadowRadius = 0.0f;
    _nSoftShadowLights = 0;
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



void Scene::setAmbientLight( Color ambientLight )
{
    _ambientLight = ambientLight;
}



void Scene::setBackgroundColor( Color backgroungColor )
{
    _backgroundColor = backgroungColor;
}



void Scene::addLight( Light* light )
{
    _lights.push_back( light );
    
    Color color = light->getDiffuse();    
    color /= _nSoftShadowLights;
    
    float delta = _softShadowRadius / _nSoftShadowLights;         
    
    // Cria luzes para efeito de soft shadow
    for (int i = 1; i <= _nSoftShadowLights; i++)
    {                       
        // Coloca uma luz de cada lado, em cada direção
        for (int j = -1; j != 1; j = 1 )
        {
            // Cria luzes no eixo x
            Vector4D positionX = light->getPosition();
            Vector4D positionY = positionX;
            Vector4D positionZ = positionX;
            
            positionX.x += i*j*delta;
            positionY.y += i*j*delta;
            positionZ.z += i*j*delta;

            Light* lightX = new Light( positionX, color );
            Light* lightY = new Light( positionY, color );
            Light* lightZ = new Light( positionZ, color );
            _softShadowLights.push_back( lightX );
            _softShadowLights.push_back( lightY );
            _softShadowLights.push_back( lightZ );
        }
    }
}



void Scene::addObject( Object* object )
{
    _objects.push_back( object );
}



void Scene::setSoftShadow( int nSoftLights, float softShadowRadius )
{
    _nSoftShadowLights = nSoftLights;
    _softShadowRadius = softShadowRadius;
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
            Color pixelColor;
            computeRayColor( ray, pixelColor, 0 );
            imgSetPixel3f( image, x, y, pixelColor.r, pixelColor.g, pixelColor.b );
        }
    }
    
    return image;
}



void Scene::computeRayColor( Ray ray, Color& colorOut, int depth )
{
    Vector4D point;
    Vector4D normal;
    int objectID;
    
    if (!computeNearestRayIntersection( ray, point, normal, objectID ))
    {
        // no interceptions
        colorOut = _backgroundColor; 
        return;
    }
    
    // Get shaded color of the objects material
    shade( ray, objectID, normal, point, colorOut );
    
    Object* object = _objects[objectID];
    Material* material = _materials[object->getMaterialId()];
    
    // Get reflection color
    double reflectionFactor = material->getReflectionFactor();
    if( reflectionFactor > 0.0 )
    {
        addReflectionComponent( object->getMaterialId(), ray, normal, point, colorOut, depth );
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



void Scene::toggleAmbient( bool isActive )
{
    _ambient = isActive;
}
    


void Scene::toggleDiffuse( bool isActive )
{
    _diffuse = isActive;
}



void Scene::toggleSpecular( bool isActive )
{
    _specular = isActive;
}
    


void Scene::toggleShadow( bool isActive )
{
     _shadow = isActive;
}
    


void Scene::toggleReflection( bool isActive )
{
    _reflection = isActive;
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
        
        if (intersected && (t < tMin) && (t > 0.01))
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



void Scene::shade( Ray& ray, int objectID, Vector4D& normal, Vector4D& point, Color& colorOut )
{
    colorOut.set( 0.0f, 0.0f, 0.0f );
    Object* object = _objects[objectID];
    int materialID = object->getMaterialId();
    
    addAmbienteComponent( materialID, colorOut );
    
    int numLights = _lights.size();
    
    for (int lightID = 0; lightID < numLights; ++lightID)
    {
        if (!inShadow( point, lightID, objectID ))
        {
            addLambertianComponent( materialID, lightID, normal, point, colorOut );
            addSpecularComponent( ray, materialID, lightID, normal, point, colorOut );
        }
    }
}



void Scene::addReflectionComponent( int materialID, Ray& ray, Vector4D& normal, Vector4D& point, Color& colorOut, int depth )
{
    // if reflections are not being taken in consideration. return.
    if( (!_reflection) || (depth > MAX_DEPTH) ) return;
    
    // create the reflected ray
    Ray reflectedRay;
    reflectedRay.origin = point;
    reflectedRay.direction = reflect( normal, -ray.direction );
    
    // compute the color of the reflected ray
    Color reflectedColor;
    computeRayColor( reflectedRay,  reflectedColor, depth+1 );
    
    // get reflection factor
    double reflectionFactor = _materials[materialID]->getReflectionFactor();
    
    // mix the two colors
    colorOut += ( reflectionFactor ) * reflectedColor;
}



void Scene::addAmbienteComponent( int materialID, Color& colorOut )
{
    // check if the ambient component should be added
    if( !_ambient ) return;
    
    // recupera cor difusa do material
    Color diffuse = _materials[materialID]->getDiffuse();
        
    // iluminação ambiente
    colorOut = _ambientLight * diffuse;
}



bool Scene::inShadow( Vector4D& point, int lightID, int objectID )
{
    // check if shadow is off
    if( !_shadow ) return false;
    
    // create a ray from the point to the light source
    Ray toLight;
    toLight.origin = point;
    toLight.direction = _lights[lightID]->getPosition() - toLight.origin;    
    double tLight = toLight.direction.norm();        
    toLight.direction.normalize();
    
    unsigned int nObjects = _objects.size();
    for (unsigned int id = 0; id < nObjects; id++)
    {
        if (id == objectID)
            continue;
        
        double t;
        Object* currentObject = _objects[id];
        
        if (currentObject->computeRayIntersection( toLight, t ) && t > 0.01 && t < tLight)
        {
            return true;
        }        
    }
    
    return false;
}



void Scene::addLambertianComponent( int materialID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut )
{
    // check if lambertian is ON
    if (!_diffuse) return;
    
    Vector4D lightDir = _lights[lightID]->getPosition() - point;
    lightDir.normalize();
    double cosTheta = dot( normal, lightDir );

    if (cosTheta < 0)
        cosTheta = 0.0;

    Color lightColor = _lights[lightID]->getDiffuse();
    Color diffuse = _materials[materialID]->getDiffuse();
    
    // iluminação lambertiana
    colorOut   += diffuse * lightColor * cosTheta;
}


void Scene::addSpecularComponent( Ray& ray, int materialID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut )
{
    // check if specular is ON
    if (!_specular) return;
    
    Vector4D lightDir = _lights[lightID]->getPosition() - point;
    lightDir.normalize();

    Color lightColor = _lights[lightID]->getDiffuse();

    // componente especular
    float exponent;
    Color specular = _materials[materialID]->getSpecular( exponent );

    Vector4D r = reflect( normal, lightDir );
    r.normalize();

    Vector4D eyeDir = ray.origin - point;
    eyeDir.normalize();

    float cosAlpha = dot(r, eyeDir);
    
    if (cosAlpha < 0.0)
        return;
    
    float specularCoeficient = pow( cosAlpha, exponent );
    
    colorOut += specular * lightColor * specularCoeficient;
}