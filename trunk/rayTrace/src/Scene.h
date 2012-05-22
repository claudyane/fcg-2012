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
#include "Color.h"

class Scene
{
    public:
        Scene();
        
        virtual ~Scene();

        void setCamera( Camera* camera );

        void setAmbientLight( Color ambientLight );

        void setBackgroundColor( Color backgroungColor );

        void addLight( Light* light );

        void addObject( Object* object );

        Image* render();

        void addMaterial( Material* material );

        Material* getMaterial( int index );               
                
        void toggleAmbient( bool isActive );
        
        void toggleDiffuse( bool isActive );
        
        void toggleSpecular( bool isActive );
        
        void toggleShadow( bool isActive );
        
        void toggleReflection( bool isActive );
        
    private:

        bool computeNearestRayIntersection( Ray ray, Vector4D& point, Vector4D& normal, int& objectID );        
        
        void computeRayColor( Ray ray, Color& colorOut, int depth );
        
        void shade( Ray& ray, int objectID, Vector4D& normal, Vector4D& point, Color& colorOut );
        
        bool inShadow( Vector4D& point, int lightID, int objectID );
        
        void addLambertianComponent( int materialID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut );

        void addSpecularComponent( Ray& ray, int materialID, int lightID, Vector4D& normal, Vector4D& point, Color& colorOut );
        
        void addAmbienteComponent( int materialID, Color& colorOut );
        
        void addReflectionComponent( int materialID, Ray& ray, Vector4D& normal, Vector4D& point, Color& colorOut, int depth );
        
        std::vector<Object*> _objects;
        std::vector<Light*> _lights;
        std::vector<Light*> _softShadowLights;
        std::vector<Material*> _materials;
        Camera* _camera;

        Color _ambientLight;
        Color _backgroundColor;
        
        bool _ambient;
        bool _diffuse;
        bool _specular;
        bool _shadow;
        bool _reflection;
        bool _softShadow;
        
        float _softShadowRadius;
        int _nSoftShadowLights;
};

#endif /* SCENE_H_ */
