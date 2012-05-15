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