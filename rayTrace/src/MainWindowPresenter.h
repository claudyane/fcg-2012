/*
 * MainWindowPresenter.h
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#ifndef MAINWINDOWPRESENTER_H_
#define MAINWINDOWPRESENTER_H_

#include "Image.h"

//stub forward declaration
class Scene;

class MainWindowPresenter
{
public:
    MainWindowPresenter();
    virtual ~MainWindowPresenter();
    void buildScene();
    void renderScene();
    Image* getImage();
    
private:
    Image* _image;
    Scene* _scene;
};

#endif /* MAINWINDOWPRESENTER_H_ */
