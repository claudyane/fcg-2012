/*
 * MainWindowPresenter.cpp
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#include "MainWindowPresenter.h"
#include "MainWindow.h"
#include "Scene.h"
#include <gdk/gdkkeysyms.h>


MainWindowPresenter::MainWindowPresenter()
{
    _image = NULL;
}



MainWindowPresenter::~MainWindowPresenter()
{
    if (_image)
        imgDestroy( _image );
}



Image* MainWindowPresenter::getImage()
{
    return _image;
}



void MainWindowPresenter::keyPress( gint key )
{
    if (!_scene.getCamera())
        return;
    
    Camera* camera = _scene.getCamera();
    
    switch (key)
    {
        case GDK_KEY_X:
            camera->incX( 10.0 );
            break;
            
        case GDK_KEY_x:
            camera->incX( -10.0 );
            break;
            
        case GDK_KEY_Y:
            camera->incY( 10.0 );
            break;
            
        case GDK_KEY_y:
            camera->incY( -10.0 );
            break;
            
        case GDK_KEY_Z:
            camera->incZ( 10.0 );
            break;
            
        case GDK_KEY_z:
            camera->incZ( -10.0 );
            break;
        
        case GDK_KEY_R:
            camera->rotateY( 0.5 );
            break;
            
        case GDK_KEY_r:
            camera->rotateY( -0.5 );
            break;
    }
    
    render();
}



bool MainWindowPresenter::loadFile( std::string filename )
{
    if (_image)
        imgClear( _image, 0.0f, 0.0f, 0.0f );
       
    _scene.loadScene( filename );        
    
    render();
    
    return true;
}



void MainWindowPresenter::render()
{
    if (_image)
        imgDestroy( _image );
    
    _image = _scene.render();
}
