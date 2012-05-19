/* 
 * File:   Box.h
 * Author: eliana
 *
 * Created on May 18, 2012, 9:17 PM
 */

#ifndef BOX_H
#define	BOX_H

#include "Material.h"
#include "Vector4D.h"
#include "Ray.h"
#include "Object.h"

class Box : public Object
{
public:
    
    Box( Vector4D min, Vector4D max );
    
    virtual ~Box();
    
    bool computeRayIntersection( Ray ray, double& t );
    
    Vector4D getNormal( Vector4D& point );
    
    void setMaterial( int materialId );
    
    int getMaterialId();
    
private:

    int _materialId;
    
    Vector4D _min;
    
    Vector4D _max;
};

#endif	/* BOX_H */

