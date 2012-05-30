/* 
 * File:   Scene.h
 * Author: walther
 *
 * Created on May 28, 2012, 5:00 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#include "Volume.h"
#include "Camera.h"
#include "Image.h"

class Scene
{
public:
    
    Scene();
        
    virtual ~Scene();

    void setVolume( Volume* volume );
    
    void setCamera( Camera* camera );

    void setBackgroundColor( Color backgroungColor );

    Image* render();
    
private:
    
    bool computeNearestRayIntersection( Ray ray, Vector4D& point, Vector4D& normal, int& objectID );        
        
    void computeRayColor( Ray ray, Color& colorOut );
    
    Volume* _volume;
    Color _backgroundColor;
    Camera* _camera;
};

#endif	/* SCENE_H */

