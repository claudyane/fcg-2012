#include "Triangle.h"


Triangle::Triangle( Vector4D v0, Vector4D v1, Vector4D v2, Vector4D tex0, Vector4D tex1, Vector4D tex2 )
{
    _v0 = v0;
    _v1 = v1;
    _v2 = v2;
    
    _tex0 = tex0;
    _tex1 = tex1;
    _tex2 = tex2;
}
    


Triangle::~Triangle()
{
    
}



bool Triangle::computeRayIntersection( Ray ray, double& t )
{
    Vector4D u = _v1 - _v0;
    Vector4D v = _v2 - _v0;
    Vector4D normal = cross( u, v );
    normal.normalize();
    
    // calcula interseção com o plano do triangulo
    t = dot( (_v0 - ray.origin), normal ) / dot( ray.direction, normal ) ;    
    Vector4D point = ray.origin + t*ray.direction;
        
    // testa se está dentro do triangulo
    double dotValue = dot( normal, cross(u, (point-_v0)) );
    
    if (dotValue < 0.0)
        return false;
    
    dotValue = dot( normal, cross( (_v2-_v1), (point-_v1)) );
    
    if (dotValue < 0.0)
        return false;
    
    dotValue = dot( normal, cross( (_v0-_v2), (point-_v2)) );
    
    if (dotValue < 0.0)
        return false;
    
    return true;  
}



Vector4D Triangle::getNormal( Vector4D& point )
{
    Vector4D normal = cross( (_v1 -_v0), (_v2 -_v0) );
    normal.normalize();
    
    return normal;
}



void Triangle::setMaterial( int materialId )
{
    _materialId = materialId;
}



int Triangle::getMaterialId()
{
    return _materialId;
}

