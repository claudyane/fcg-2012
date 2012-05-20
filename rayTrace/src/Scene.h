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
                
        void toggleAmbient( bool isActive );
        
        void toggleDiffuse( bool isActive );
        
        void toggleSpecular( bool isActive );
        
        void toggleShadow( bool isActive );
        
        void toggleReflection( bool isActive );
        
    private:

        bool computeNearestRayIntersection( Ray ray, Vector4D& point, Vector4D& normal, int& objectID );        
        
        void computeRayColor( Ray ray, float& rOut, float& gOut, float& bOut, int count );
        
        void shade( int objectID, Vector4D& normal, Vector4D& point, float& rOut, float& gOut, float& bOut );
        
        bool inShadow( Vector4D& point, int lightID, int objectID );
        
        void addLambertianComponent( int materialID, int lightID, Vector4D& normal, Vector4D& point, float& red, float& green, float& blue );

        void addSpecularComponent( int materialID, int lightID, Vector4D& normal, Vector4D& point, float& red, float& green, float& blue );
        
        void addAmbienteComponent( int materialID, float& red, float& green, float& blue );
        
        void addReflectionComponent( int materialID, Ray& ray, Vector4D& normal, Vector4D& point, float& red, float& green, float& blue, int depth );
        
        std::vector<Object*> _objects;
        std::vector<Light*> _lights;
        std::vector<Material*> _materials;
        Camera* _camera;

        Vector4D _ambientLight;
        Vector4D _backgroundColor;
        
        bool _ambient;
        bool _diffuse;
        bool _specular;
        bool _shadow;
        bool _reflection;

};

#endif /* SCENE_H_ */
