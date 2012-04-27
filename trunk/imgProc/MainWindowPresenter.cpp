/* 
 * File:   MainWindowPresenter.cpp
 * Author: walther
 * 
 * Created on April 13, 2012, 6:38 PM
 */

#include <stdio.h>
#include <complex>
#include "image.h"

#include "MainWindowPresenter.h"

MainWindowPresenter::MainWindowPresenter()
{
    _srcImg = 0;
    _fourierImg = 0;
    _dstImg = 0;
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
    
    if( !_srcImg || !_dstImg )
    {
        printf("nao abriu: %s\n", path);
    }
}



void MainWindowPresenter::saveImage(char* path)
{
    if( !path ) return;
    
    imgWriteBMP( path, _dstImg );
}



void MainWindowPresenter::resetImage()
{
    imgDestroy( _dstImg );
    _dstImg = imgCopy( _srcImg );
}



void MainWindowPresenter::applyGrayscale()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgGrey( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyGauss()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgCopy( _dstImg );
    
    imgGauss( _dstImg, auxImg );
}



void MainWindowPresenter::applyMedian()
{
    if( !_dstImg ) return;
    
    imgMedian( _dstImg );
}



void MainWindowPresenter::applyEdges()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgEdges( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyBinOtsu()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgBinOtsu( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyBinOhbuchi()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgBinOhbuchi( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyFourrier( bool phase )
{
    int w = imgGetWidth(_dstImg);
    int h = imgGetHeight(_dstImg);
    
    if (!_dstImg) return;
    if (imgGetDimColorSpace(_dstImg) != 1) return;
    
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
}



void MainWindowPresenter::applyShift()
{
    if( !_dstImg ) return;
    
    Image* auxImg = imgShift( _dstImg );
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}



void MainWindowPresenter::applyGamma( float factor )
{
    if( !_dstImg ) return;
    
    imgGamma( _dstImg, factor );
}



void MainWindowPresenter::applyNormalize()
{
    if( !_dstImg ) return;
    
    imgNormalize( _dstImg );
}



void MainWindowPresenter::applyInverseFourier()
{
    if( !_fourierImg ) return;
    if( !_dstImg ) return;
    
    Image* auxImg = imgInverseFourier( _fourierImg, imgGetWidth( _dstImg ), imgGetHeight( _dstImg ) );
    
    imgDestroy( _dstImg );
    _dstImg = imgCopy( auxImg );
}
