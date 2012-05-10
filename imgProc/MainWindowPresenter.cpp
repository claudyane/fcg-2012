/* 
 * File:   MainWindowPresenter.cpp
 * Author: walther
 * 
 * Created on April 13, 2012, 6:38 PM
 */

#include <stdio.h>
#include <complex>
#include <gtk/gtkstyle.h>
#include <gtk/gtk.h>
#include "image.h"

#include "MainWindowPresenter.h"

MainWindowPresenter::MainWindowPresenter( GtkWindow* window )
{
    _srcImg = 0;
    _fourierImg = 0;
    _dstImg = 0;
    _window = window;
}



MainWindowPresenter::~MainWindowPresenter()
{
}



Image* MainWindowPresenter::getDstImage()
{
    return _dstImg;
}



Image* MainWindowPresenter::getSrcImage()
{
    return _srcImg;
}



void MainWindowPresenter::openImage( char* path )
{
    if( _fourierImg )
    {
        delete[] _fourierImg;
        _fourierImg = 0;
    }
    
    if( _srcImg )
    {
        imgDestroy( _srcImg );
        imgDestroy( _dstImg );
    }
    
    _srcImg = imgReadBMP( path );
    _dstImg = imgCopy( _srcImg );
    
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "The image was not loaded correctly" );
        return;
    }
}



void MainWindowPresenter::saveImage(char* path)
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    if( !path ) return;
    
    imgWriteBMP( path, _dstImg );
}



void MainWindowPresenter::resetImage()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    if( _fourierImg )
    {
        delete[] _fourierImg;
        _fourierImg = 0;
    }
    
    imgDestroy( _dstImg );
    _dstImg = imgCopy( _srcImg );
}



void MainWindowPresenter::applyGrayscale()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgGrey( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyGauss()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgCopy( _dstImg );
    
    imgGauss( _dstImg, auxImg );
}



void MainWindowPresenter::applyMedian()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    imgMedian( _dstImg );
}



void MainWindowPresenter::applyEdges()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgEdges( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyBinOtsu()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgBinOtsu( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyBinOhbuchi()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgBinOhbuchi( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyFourrier( bool phase )
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    if (imgGetDimColorSpace(_dstImg) != 1)
    {
        showErrorMessage( "The image must be in gray scale" );
        return;
    }
    
    int w = imgGetWidth(_dstImg);
    int h = imgGetHeight(_dstImg);
    
    if (!_fourierImg)
    {
        _fourierImg = imgFourier( _dstImg );
    }
    
    imgDestroy( _dstImg );
    _dstImg = imgCreate( w, h, 1 );
    
    int size = w*h;
    float* buf = imgGetData( _dstImg );
    
    for (int i = 0; i < size; ++i)
    {
        if( phase )
        {
            buf[i] = std::arg( _fourierImg[i] );
        }
        else
        {
            buf[i] = std::abs( _fourierImg[i] );
        }
    }
    
    applyShift();
    applyNormalize();
}



void MainWindowPresenter::applyShift()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgShift( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyGamma( float factor )
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    imgGamma( _dstImg, factor );
}



void MainWindowPresenter::applyNormalize()
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    imgNormalize( _dstImg );
}



void MainWindowPresenter::applyInverseFourier()
{
    if( !_fourierImg )
    {
        showErrorMessage( "The Fourier transform was not calculated" );
        return;
    }
    
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    Image* auxImg = imgInverseFourier( _fourierImg, imgGetWidth( _dstImg ), imgGetHeight( _dstImg ) );
    
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyHighPass( float radius )
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    if( !_fourierImg )
    {
        showErrorMessage( "The Fourier transform was not calculated" );
        return;
    }
    
    int w = imgGetWidth( _dstImg );
    int h = imgGetHeight( _dstImg );
    int radius_2 = radius * radius;
    
    //create black and white image to hold the filter
    Image* filter = imgCreate( w, h, 1 );
    
    for (int x = 0; x < w; ++x)
    {
        int x_2 = (x-w/2) * (x-w/2);
        
        for (int y = 0; y < h; ++y)
        {
            int y_2 = (y-h/2) * (y-h/2);
            
            //if the pixel is in the circle, paint it black
            if (( x_2 + y_2 ) <= radius_2)
            {
                imgSetPixel3f( filter, x, y, 0.0f, 0.0f, 0.0f );
            }
            else
            {
                imgSetPixel3f( filter, x, y, 1.0f, 1.0f, 1.0f );
            }
        }
    }
    
    // shift the filter to match the transform
    Image* filterShift = imgShift( filter );
    
    int total = w * h;
    float* filterBuf = imgGetData( filter );
    float* filterShiftBuf = imgGetData( filterShift );
    float* dstBuf = imgGetData( _dstImg );
    for (int i = 0; i < total;  ++i)
    {
        _fourierImg[i] *= (double)filterShiftBuf[i];
        dstBuf[i] *= (double)filterBuf[i];
    }
    
    imgDestroy( filter );
    imgDestroy( filterShift );
}



void MainWindowPresenter::applyLowPass( float radius )
{
    if( (!_dstImg) || (!_srcImg) )
    {
        showErrorMessage( "No loaded image" );
        return;
    }
    
    if( !_fourierImg )
    {
        showErrorMessage( "The Fourier transform was not calculated" );
        return;
    }
    
    int w = imgGetWidth( _dstImg );
    int h = imgGetHeight( _dstImg );
    int radius_2 = radius * radius;
    
    //create black and white image to hold the filter
    Image* filter = imgCreate( w, h, 1 );
    
    for (int x = 0; x < w; ++x)
    {
        int x_2 = (x-w/2) * (x-w/2);
        
        for (int y = 0; y < h; ++y)
        {
            int y_2 = (y-h/2) * (y-h/2);
            
            //if the pixel is out of the circle, paint it black
            if (( x_2 + y_2 ) > radius_2)
            {
                imgSetPixel3f( filter, x, y, 0.0f, 0.0f, 0.0f );
            }
            else
            {
                imgSetPixel3f( filter, x, y, 1.0f, 1.0f, 1.0f );
            }
        }
    }
    
    // shift the filter to match the transform
    Image* filterShift = imgShift( filter );
    
    int total = w * h;
    float* filterBuf = imgGetData( filter );
    float* filterShiftBuf = imgGetData( filterShift );
    float* dstBuf = imgGetData( _dstImg );
    for (int i = 0; i < total;  ++i)
    {
        _fourierImg[i] *= (double)filterShiftBuf[i];
        dstBuf[i] *= (double)filterBuf[i];
    }
    
    imgDestroy( filter );
    imgDestroy( filterShift );
}



void MainWindowPresenter::applyBandPass( float inRadius, float outRadius )
{
    applyHighPass( inRadius );
    applyLowPass( outRadius );
}



void MainWindowPresenter::showErrorMessage( const char* message )
{
    GtkWidget* dialog = gtk_message_dialog_new (GTK_WINDOW(_window),
                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                GTK_MESSAGE_ERROR,
                                GTK_BUTTONS_CLOSE,
                                "ERROR:\n%s",
                                message);
    gtk_dialog_run ( GTK_DIALOG(dialog) );
    gtk_widget_destroy (dialog);
}
