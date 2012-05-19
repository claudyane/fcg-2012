/* 
 * File:   Box.cpp
 * Author: eliana
 * 
 * Created on May 18, 2012, 9:17 PM
 */

#include <float.h>
#include <cstdlib>
#include "Box.h"
#include "Ray.h"
#include <cmath>

Box::Box( Vector4D min, Vector4D max )
{
    _min = min;
    _max = max;
}



Box::~Box( )
{
}



bool Box::computeRayIntersection( Ray ray, double& t )
{
    float tMin = DBL_MAX, tAux = DBL_MAX;
    
    //Procura interseção nos planos x = xmin e x = xmax
    if (ray.direction.x != 0.0)
    {
        // x = xmin
        tAux = (_min.x - ray.origin.x)/ray.direction.x;
                
        float y = ray.origin.y + tAux * ray.direction.y;
        float z = ray.origin.z + tAux * ray.direction.z;        
        if ( y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux;            
        
        // x = xmax
        tAux = (_max.x - ray.origin.x)/ray.direction.x;
       
        y = ray.origin.y + tAux * ray.direction.y;
        z = ray.origin.z + tAux * ray.direction.z;        
        if ( y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux; 
    }
    
    //Procura interseção nos planos y = ymin e y = ymax
    if (ray.direction.y != 0.0)
    {
        // y = ymin
        tAux = (_min.y - ray.origin.y)/ray.direction.y;
                
        float x = ray.origin.x + tAux * ray.direction.x;
        float z = ray.origin.z + tAux * ray.direction.z;        
        if ( x >= _min.x && x <= _max.x && z >= _min.z && z <= _max.z && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux;            
        
        // y = ymax
        tAux = (_max.y - ray.origin.y)/ray.direction.y;
       
        x = ray.origin.x + tAux * ray.direction.x;
        z = ray.origin.z + tAux * ray.direction.z;        
        if ( x >= _min.x && x <= _max.x && z >= _min.z && z <= _max.z && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux; 
    }
    
    //Procura interseção nos planos z = zmin e z = zmax
    if (ray.direction.z != 0.0)
    {
        // z = zmin
        tAux = (_min.z - ray.origin.z)/ray.direction.z;
                
        float y = ray.origin.y + tAux * ray.direction.y;
        float x = ray.origin.x + tAux * ray.direction.x;        
        if ( y >= _min.y && y <= _max.y && x >= _min.x && x <= _max.x && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux;            
        
        // z = zmax
        tAux = (_max.z - ray.origin.z)/ray.direction.z;
       
        y = ray.origin.y + tAux * ray.direction.y;
        x = ray.origin.x + tAux * ray.direction.x;        
        if ( y >= _min.y && y <= _max.y && x >= _min.x && x <= _max.x && tAux > 0.0 && tAux < tMin) // Testa se está dentro da caixa
            tMin = tAux; 
    }
    
    t = tMin;
    
    return (tMin < DBL_MAX);
}



Vector4D Box::getNormal( Vector4D& point )
{
    // Testa se está no plano x = xmin
    if (fabs(point.x - _min.x) < 0.00001)
        return Vector4D( -1.0, 0.0, 0.0, 1.0 );
    
    // Testa se está no plano x = xmax
    if (fabs(point.x - _max.x) < 0.00001)
        return Vector4D( 1.0, 0.0, 0.0, 1.0 );
    
    // Testa se está no plano y = ymin
    if (fabs(point.y - _min.y) < 0.00001)
        return Vector4D( 0.0, -1.0, 0.0, 1.0 );
    
    // Testa se está no plano y = ymax
    if (fabs(point.y - _max.y) < 0.00001)
        return Vector4D( 0.0, 1.0, 0.0, 1.0 );
        
    // Testa se está no plano z = zmin
    if (fabs(point.z - _min.z) < 0.00001)
        return Vector4D( 0.0, 0.0, -1.0, 1.0 );
    
    // Testa se está no plano z = xmax
    if (fabs(point.z - _max.z) < 0.00001)
        return Vector4D( 0.0, 0.0, 1.0, 1.0 );
    
    return Vector4D( 0.0, 0.0, 0.0, 1.0 );
}



void Box::setMaterial( int materialId )
{
   _materialId = materialId;
}



int Box::getMaterialId()
{
    return _materialId;
}

