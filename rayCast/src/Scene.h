/* 
 * File:   Scene.h
 * Author: walther
 *
 * Created on May 28, 2012, 5:00 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#include "Volume.h"


class Scene
{
public:
    
    Scene();
    Scene(const Scene& orig);
    virtual ~Scene();
    
private:
    
    Volume* _volume;
};

#endif	/* SCENE_H */

