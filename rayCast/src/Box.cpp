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
    // garante coerencia
    if (min.x < max.x)
    {
        _min.x = min.x;
        _max.x = max.x;        
    }
    else
    {
        _min.x = max.x;
        _max.x = min.x;        
    }

    if (min.y < max.y)
    {
        _min.y = min.y;
        _max.y = max.y;        
    }
    else
    {
        _min.y = max.y;
        _max.y = min.y;        
    }
    
    if (min.z < max.z)
    {
        _min.z = min.z;
        _max.z = max.z;        
    }
    else
    {
        _min.z = max.z;
        _max.z = min.z;        
    }
}



Box::~Box( )
{
}



bool Box::computeRayIntersection( Ray ray, double& tIn, double& tOut )
{
    double tMin = DBL_MAX;
    double tMax = -1.0;
    double tAux = DBL_MAX;
    
    //Procura interseção nos planos x = xmin e x = xmax
    if (ray.direction.x != 0.0)
    {
        // x = xmin
        tAux = (_min.x - ray.origin.x)/ray.direction.x;
                
        double y = ray.origin.y + tAux * ray.direction.y;
        double z = ray.origin.z + tAux * ray.direction.z;        
        if ( y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z && tAux > 0.0 )
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        }
        
        // x = xmax
        tAux = (_max.x - ray.origin.x)/ray.direction.x;
       
        y = ray.origin.y + tAux * ray.direction.y;
        z = ray.origin.z + tAux * ray.direction.z;        
        if ( y >= _min.y && y <= _max.y && z >= _min.z && z <= _max.z && tAux > 0.0 ) // Testa se está dentro da caixa
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        }
    }
    
    //Procura interseção nos planos y = ymin e y = ymax
    if (ray.direction.y != 0.0)
    {
        // y = ymin
        tAux = (_min.y - ray.origin.y)/ray.direction.y;
                
        double x = ray.origin.x + tAux * ray.direction.x;
        double z = ray.origin.z + tAux * ray.direction.z;        
        if ( x >= _min.x && x <= _max.x && z >= _min.z && z <= _max.z && tAux > 0.0 ) // Testa se está dentro da caixa
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        }           
        
        // y = ymax
        tAux = (_max.y - ray.origin.y)/ray.direction.y;
       
        x = ray.origin.x + tAux * ray.direction.x;
        z = ray.origin.z + tAux * ray.direction.z;        
        if ( x >= _min.x && x <= _max.x && z >= _min.z && z <= _max.z && tAux > 0.0 ) // Testa se está dentro da caixa
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        } 
    }
    
    //Procura interseção nos planos z = zmin e z = zmax
    if (ray.direction.z != 0.0)
    {
        // z = zmin
        tAux = (_min.z - ray.origin.z)/ray.direction.z;
                
        double y = ray.origin.y + tAux * ray.direction.y;
        double x = ray.origin.x + tAux * ray.direction.x;        
        if ( y >= _min.y && y <= _max.y && x >= _min.x && x <= _max.x && tAux > 0.0 ) // Testa se está dentro da caixa
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        }          
        
        // z = zmax
        tAux = (_max.z - ray.origin.z)/ray.direction.z;
       
        y = ray.origin.y + tAux * ray.direction.y;
        x = ray.origin.x + tAux * ray.direction.x;        
        if ( y >= _min.y && y <= _max.y && x >= _min.x && x <= _max.x && tAux > 0.0 ) // Testa se está dentro da caixa
        {
            if (tAux < tMin) tMin = tAux;
            if (tAux > tMax) tMax = tAux;
        } 
    }
    
    tIn  = tMin;
    tOut = tMax;
    
    return ( ( tMin < DBL_MAX ) && ( tMax > -1.0 ) );
}



Vector4D Box::getNormal( Vector4D& point )
{
    // Testa se está no plano x = xmin
    if (fabs(point.x - _min.x) < 0.0001)
        return Vector4D( -1.0, 0.0, 0.0, 1.0 );
    
    // Testa se está no plano x = xmax
    if (fabs(point.x - _max.x) < 0.0001)
        return Vector4D( 1.0, 0.0, 0.0, 1.0 );
    
    // Testa se está no plano y = ymin
    if (fabs(point.y - _min.y) < 0.0001)
        return Vector4D( 0.0, -1.0, 0.0, 1.0 );
    
    // Testa se está no plano y = ymax
    if (fabs(point.y - _max.y) < 0.0001)
        return Vector4D( 0.0, 1.0, 0.0, 1.0 );
        
    // Testa se está no plano z = zmin
    if (fabs(point.z - _min.z) < 0.0001)
        return Vector4D( 0.0, 0.0, -1.0, 1.0 );
    
    // Testa se está no plano z = xmax
    if (fabs(point.z - _max.z) < 0.0001)
        return Vector4D( 0.0, 0.0, 1.0, 1.0 );
    
    return Vector4D( 0.0, 0.0, 0.0, 1.0 );
}


Vector4D Box::getMin()
{
    return _min;
}



Vector4D Box::getMax()
{
    return _max;
}

