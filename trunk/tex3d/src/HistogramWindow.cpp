#include "HistogramWindow.h"


HistogramWindow::HistogramWindow()
{   
    _histogram = new Histogram( 768, 512 );
}



HistogramWindow::~HistogramWindow( )
{
    if (_histogram)
        delete _histogram;
}



void HistogramWindow::show()
{
    
}
    


void HistogramWindow::hide()
{

}



void HistogramWindow::setVolume( Volume* volume )
{
    _histogram->setVolume( volume );
}
