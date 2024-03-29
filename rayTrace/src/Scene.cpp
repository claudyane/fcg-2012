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
#include <time.h>
#include <stdlib.h>

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
    
    Vector4D position;
    
    // Cria luzes para efeito de soft shadow
    for (int i = 0; i <  _nSoftShadowLights; ++i)
    {
        float phi = (((float)rand())/(float)RAND_MAX) * M_PI;
        float theta = (((float)rand())/(float)RAND_MAX) * M_PI * 2.0;
        
        position = light->getPosition();
        
        position.x += _softShadowRadius * cos( theta ) * sin( phi );
        position.y += _softShadowRadius * sin( theta ) * sin( phi );
        position.z += _softShadowRadius * cos( phi );
        
        _softShadowLights.push_back( new Light( position, color ) );
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
    
    if( _antiAlias )
    {
        #pragma omp parallel for
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                Ray ray0 = _camera->computeRay( x + 0.25, y + 0.25 );
                Ray ray1 = _camera->computeRay( x + 0.25, y - 0.25 );
                Ray ray2 = _camera->computeRay( x - 0.25, y + 0.25 );
                Ray ray3 = _camera->computeRay( x - 0.25, y - 0.25 );

                Color pixelColor0;
                Color pixelColor1;
                Color pixelColor2;
                Color pixelColor3;

                computeRayColor( ray0, pixelColor0, 0 );
                computeRayColor( ray1, pixelColor1, 0 );
                computeRayColor( ray2, pixelColor2, 0 );
                computeRayColor( ray3, pixelColor3, 0 );

                Color pixelColor = ( pixelColor0 * 0.25 + pixelColor1 * 0.25 + pixelColor2 * 0.25 + pixelColor3 * 0.25 );

                imgSetPixel3f( image, x, y, pixelColor.r, pixelColor.g, pixelColor.b );
            }
        }
    }
    else
    {
        #pragma omp parallel for
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                Ray ray = _camera->computeRay( x + 0.25, y + 0.25 );
                Color pixelColor;
                computeRayColor( ray, pixelColor, 0 );
                imgSetPixel3f( image, x, y, pixelColor.r, pixelColor.g, pixelColor.b );
            }
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
        addReflectionComponent( objectID, ray, normal, point, colorOut, depth );
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



void Scene::toggleSoftShadow(bool isActive)
{
    _softShadow = isActive;
}



void Scene::toggleReflection( bool isActive )
{
    _reflection = isActive;
}



void Scene::toggleAntiAlias( bool isActive )
{
    _antiAlias = isActive;
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
    addAmbienteComponent( objectID, colorOut, point );
        
    int numLights = _lights.size();
    
    for (int lightID = 0; lightID < numLights; ++lightID)
    {
        double shadowFactor = computeShadowFactor( point, lightID, objectID );

        addLambertianComponent( objectID, lightID, normal, point, colorOut, shadowFactor );
        addSpecularComponent( ray, objectID, lightID, normal, point, colorOut, shadowFactor );
    }
}



void Scene::addReflectionComponent( int objectID, Ray& ray, Vector4D& normal, Vector4D& point, Color& colorOut, int depth )
{
    // if reflections are not being taken in consideration. return.
    if( (!_reflection) || (depth > MAX_DEPTH) ) return;
    
    int materialID = _objects[objectID]->getMaterialId();
    
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



void Scene::addAmbienteComponent( int objectID, Color& colorOut, Vector4D& point )
{
    // check if the ambient component should be added
    if( !_ambient ) return;
    
    Object* object = _objects[objectID];
    
    int materialID = object->getMaterialId();
    
    // recupera cor difusa do material
    Color diffuse = _materials[materialID]->getDiffuse( object, point );
        
    // iluminação ambiente
    colorOut = _ambientLight * diffuse;
}



double Scene::computeShadowFactor( Vector4D& point, int lightID, int objectID )
{
    // Se possui softShadow, calcula a media do numeros de luzes que iluminam o ponto
    if (_softShadow)
    {
        int nLights = _nSoftShadowLights;
        int firstLight = lightID*nLights;
        double factor = 0.0;
        for (int i = 0; i < nLights; i++)
        {
            if (!inShadow( point, _softShadowLights[firstLight + i], objectID ))
                factor += 1.0;
        }
        
        return factor/nLights;
    }
    else if (_shadow) // Se está calculando sombra simples, só precisa chamar inShadow
    {
        if (inShadow( point, _lights[lightID], objectID))
            return 0.0;
        else
            return 1.0;
    }
    
    // Se não entrou em um dos ifs não está sendo calculada sombra e o fator é 1
    return 1.0;    
}



bool Scene::inShadow( Vector4D& point, Light* light, int objectID )
{
    // check if shadow is off
    if( !_shadow ) return false;
    
    // create a ray from the point to the light source
    Ray toLight;
    toLight.origin = point;
    toLight.direction = light->getPosition() - toLight.origin;    
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



void Scene::addLambertianComponent( int objectID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut, double shadowFactor )
{
    // check if lambertian is ON
    if (!_diffuse) return;
    
    Object* object = _objects[objectID];
    
    int materialID = object->getMaterialId();
    
    Vector4D lightDir = _lights[lightID]->getPosition() - point;
    lightDir.normalize();
    double cosTheta = dot( normal, lightDir );

    if (cosTheta < 0)
        cosTheta = 0.0;

    Color lightColor = _lights[lightID]->getDiffuse();
    Color diffuse = _materials[materialID]->getDiffuse( object, point );
    
    // iluminação lambertiana
    colorOut   += diffuse * lightColor * cosTheta * shadowFactor;
}


void Scene::addSpecularComponent( Ray& ray, int objectID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut, double shadowFactor )
{
    // check if specular is ON
    if (!_specular) return;
    
    int materialID = _objects[objectID]->getMaterialId();
    
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
    
    colorOut += specular * lightColor * specularCoeficient * shadowFactor;
}