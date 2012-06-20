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
#include "Scene.h"
#include <gtk/gtk.h>

class MainWindowPresenter
{
public:
    MainWindowPresenter( GtkWidget* canvas );
    virtual ~MainWindowPresenter();
    bool loadFile( std::string filename );
    void render();
    Image* getImage();
    
    void keyPress( gint key );
private:
    Image* _image;
    Scene _scene;
    
    float _increment;
    
    
    GtkWidget* _canvas;
};

#endif /* MAINWINDOWPRESENTER_H_ */
