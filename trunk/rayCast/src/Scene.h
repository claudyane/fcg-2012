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
#include "Light.h"

class Scene
{
public:
    
    Scene();
        
    virtual ~Scene();

    bool loadScene( std::string filePath );
    
    void setVolume( Volume* volume );
    
    void setCamera( Camera* camera );
    
    Camera* getCamera();

    void setBackgroundColor( Color backgroungColor );

    Image* render();
    
private:    

    void computeRayColor( Ray ray, Color& colorOut );
    
    Volume* _volume;
    Color _backgroundColor;
    Camera* _camera;
    Light* _light;
};

#endif	/* SCENE_H */

