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

        void setCamera( Camera* camera );

        void setAmbientLight( Vector4D ambientLight );

        void setBackgroundColor( Vector4D backgroungColor );

        void addLight( Light* light );

        void addObject( Object* object );

        Image* render();

        void addMaterial( Material* material );

        Material* getMaterial( int index );
        
        bool computeNearestRayIntersection( Ray ray, Vector4D& point, Vector4D& normal, int& objectID );
        
        void shade( int materialId, Vector4D& normal, Vector4D& point, float& rOut, float& gOut, float& bOut );

    private:

        std::vector<Object*> _objects;
        std::vector<Light*> _lights;
        std::vector<Material*> _materials;
        Camera* _camera;

        Vector4D _ambientLight;
        Vector4D _backgroundColor;

};

#endif /* SCENE_H_ */