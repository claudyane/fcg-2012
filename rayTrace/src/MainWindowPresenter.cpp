/*
 * MainWindowPresenter.cpp
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#include "MainWindowPresenter.h"
#include "MainWindow.h"
#include "Scene.h"
#include "Rt4FileReader.h"


MainWindowPresenter::MainWindowPresenter()
{
    _image = NULL;
    _scene  = NULL;
}



MainWindowPresenter::~MainWindowPresenter()
{
    // TODO Auto-generated destructor stub
}



Image* MainWindowPresenter::getImage()
{
    return _image;
}



bool MainWindowPresenter::buildScene( std::string filename )
{
    if( _scene ) delete _scene;
    
    _scene = new Scene();
    
    Rt4FileReader reader;
    reader.loadScene( filename, _scene );

    return true;
}



void MainWindowPresenter::renderScene()
{
    if (_image)
        imgDestroy( _image );
        
    _image = _scene->render();
}



void MainWindowPresenter::toggleAmbient( bool isActive )
{
    if (!_scene)
        return;
    
    _scene->toggleAmbient( isActive );
}
    


void MainWindowPresenter::toggleDiffuse( bool isActive )
{
    if (!_scene)
        return;
    
    _scene->toggleDiffuse( isActive );
}



void MainWindowPresenter::toggleSpecular( bool isActive )
{
    if (!_scene)
        return;
    
    _scene->toggleSpecular( isActive );
}
    


void MainWindowPresenter::toggleShadow( bool isActive )
{
    if (!_scene)
        return;
    
     _scene->toggleShadow( isActive );
}
    


void MainWindowPresenter::toggleAntiAlias( bool isActive )
{
    if (!_scene)
        return;
    
    _scene->toggleAntiAlias( isActive );
}

