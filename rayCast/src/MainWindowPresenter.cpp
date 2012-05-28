/*
 * MainWindowPresenter.cpp
 *
 *  Created on: May 14, 2012
 *      Author: walther
 */

#include "MainWindowPresenter.h"
#include "MainWindow.h"


MainWindowPresenter::MainWindowPresenter()
{
    _image = NULL;
}



MainWindowPresenter::~MainWindowPresenter()
{
    // TODO Auto-generated destructor stub
}



Image* MainWindowPresenter::getImage()
{
    return _image;
}



bool MainWindowPresenter::loadFile( std::string filename )
{
    if (_image)
        imgClear( _image, 0.0f, 0.0f, 0.0f );
    
    return true;
}



void MainWindowPresenter::render()
{
    if (_image)
        imgDestroy( _image );
}
