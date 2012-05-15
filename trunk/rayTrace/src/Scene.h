/*
 * Scene.h
 *
 *  Created on: May 14, 2012
 *      Author: eliana
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Scene
{
    public:
        Scene();
        virtual ~Scene();

    private:

        std::vector<Object*> _objects;
        std::vector<Light*> _lights;
        std::vector<Material*> _materials;
        Camera* _camera;
};

#endif /* SCENE_H_ */
