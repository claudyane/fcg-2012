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


MainWindowPresenter::MainWindowPresenter( GtkWidget* canvas )
{
    _image = NULL;
    _increment = 1.0;
    
    _canvas = canvas;
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
            camera->incX( _increment );
            render();
            break;
            
        case GDK_KEY_x:
            camera->incX( -_increment );
            render();
            break;
            
        case GDK_KEY_Y:
            camera->incY( _increment );
            render();
            break;
            
        case GDK_KEY_y:
            camera->incY( -_increment );
            render();
            break;
            
        case GDK_KEY_Z:
            camera->incZ( _increment );
            render();
            break;
            
        case GDK_KEY_z:
            camera->incZ( -_increment );
            render();
            break;
        
        case GDK_KEY_R:
            camera->rotateY( 0.5 );
            render();
            break;
            
        case GDK_KEY_r:
            camera->rotateY( -0.5 );
            render();
            break;
            
        case GDK_KEY_Up:
            _increment += 1.0;
            printf("increment %f\n", _increment );
            break;
            
        case GDK_KEY_Down:
            _increment -= 1.0;
            printf("increment %f\n", _increment );
            break;
    }
    
    //render();
}



bool MainWindowPresenter::loadFile( std::string filename )
{
    if (_image)
        imgClear( _image, 0.0f, 0.0f, 0.0f );
       
    _scene.loadScene( filename );
    _scene.getCamera()->setManipulator( new VManipulator( _canvas ));
    
    render();
    
    return true;
}



void MainWindowPresenter::render()
{
    _scene.render();
}
