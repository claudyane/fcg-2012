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
    bool loadFile( std::string filename );
    void render();
    Image* getImage();
    
private:
    Image* _image;
    // TODO: objeto volume
};

#endif /* MAINWINDOWPRESENTER_H_ */
