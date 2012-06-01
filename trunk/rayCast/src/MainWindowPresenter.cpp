/*
 * MainWindowPresenter.cpp
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#include "MainWindowPresenter.h"
#include "MainWindow.h"
#include "Scene.h"


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



bool MainWindowPresenter::loadFile( std::string filename )
{
    if (_image)
        imgClear( _image, 0.0f, 0.0f, 0.0f );
       
    return _scene.loadScene( filename );        
}



void MainWindowPresenter::render()
{
    if (_image)
        imgDestroy( _image );
    
    _image = _scene.render();
}
