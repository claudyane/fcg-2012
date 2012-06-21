/* 
 * File:   Scene.h
 * Author: walther
 *
 * Created on May 28, 2012, 5:00 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#include <GL/gl.h>
#include "Volume.h"
#include "Camera.h"
#include "Image.h"

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

    void render();
    
    void loadTexture3D();
    
private:
    
    void drawBox();
    
    Volume* _volume;
    Color _backgroundColor;
    Camera* _camera;
    
    GLuint _textureID;
};

#endif	/* SCENE_H */
