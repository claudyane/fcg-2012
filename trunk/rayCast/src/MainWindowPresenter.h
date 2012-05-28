/*
 * MainWindowPresenter.h
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#ifndef MAINWINDOWPRESENTER_H_
#define MAINWINDOWPRESENTER_H_

#include <string>
#include "Image.h"

//stub forward declaration
class Scene;

class MainWindowPresenter
{
public:
    MainWindowPresenter();
    virtual ~MainWindowPresenter();
    bool buildScene( std::string filename );
    void renderScene();
    Image* getImage();
    
    void toggleAmbient( bool isActive );
    void toggleDiffuse( bool isActive );
    void toggleSpecular( bool isActive );
    void toggleShadow( bool isActive );
    void toggleSoftShadow( bool isActive );
    void toggleReflection( bool isActive );
    void toggleAntiAlias( bool isActive );
    
    
private:
    Image* _image;
    Scene* _scene;
};

#endif /* MAINWINDOWPRESENTER_H_ */
