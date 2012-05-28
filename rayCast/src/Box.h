/* 
 * File:   Box.h
 * Author: eliana
 *
 * Created on May 18, 2012, 9:17 PM
 */

#ifndef BOX_H
#define	BOX_H

#include "Vector4D.h"

class Box
{
public:
    
    Box( Vector4D min, Vector4D max );
    
    virtual ~Box();
    
    bool computeRayIntersection( Ray ray, double& t );
    
    Vector4D getNormal( Vector4D& point );
    
    Vector4D getMin();
    
    Vector4D getMax();
    
private:
    
    Vector4D _min;
    
    Vector4D _max;
};

#endif	/* BOX_H */

