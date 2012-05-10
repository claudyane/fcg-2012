/* 
 * File:   MainWindowPresenter.h
 * Author: walther
 *
 * Created on April 13, 2012, 6:38 PM
 */

#ifndef MAINWINDOWPRESENTER_H
#define	MAINWINDOWPRESENTER_H

#include "image.h"

/**
 * The presenter of a window is in charge of doing what is not user interaction.
 * This class holds the number crunching methodsused on the image that is being
 * processed.
 */
class MainWindowPresenter
{
public:
    /**
     * Default constructor
     */
    MainWindowPresenter( GtkWindow* window );
    
    /**
     * Default destructor
     */
    virtual ~MainWindowPresenter();
    
    /**
     * Opens a new image and releases the current opened image, if any.
     * @param path is the path to the image to be opened.
     */
    void openImage( char* path );
    
    /**
     * Saves the currently edited image
     * @param path is the path to the image to be saved.
     */
    void saveImage( char* path );
    
    /**
     * Resets the edited image to its original form.
     */
    void resetImage();
    
    /**
     * Gives direct access to the destination image object the class holds.
     * @return pointer to Image
     */
    Image* getDstImage();
    
    /**
     * Gives direct access to the source image object the class holds.
     * @return pointer to Image
     */
    Image* getSrcImage();
    
    /**
     * Turns the image in gray scale
     */
    void applyGrayscale();
    
    /**
     * Applies the Gaussian filter on the image.
     */
    void applyGauss();
    
    /**
     * Applies the Median filter on the image
     */
    void applyMedian();
    
    /**
     * Applies the Edge filter on the image
     */
    void applyEdges();
    
    /**
     * Applies the N. Otsu filter for binarization on the image
     */
    void applyBinOtsu();
    
    /**
     * Applies the E. Ohbuchi filter for binarization on the image.
     */
    void applyBinOhbuchi();
    
    /**
     * Applies the Fourier Transform on the image.
     * 
     * @param phase is true if the image should output the phase of the 
     * transform, and false if it should output the magnitude.
     */
    void applyFourrier( bool phase );
    
    /**
     * Shifts the image, wrapping the edges.
     * This function shifts the image in a way that the four corners meet in the
     * center.
     */
    void applyShift();
    
    /**
     * Applies gamma factor
     * @param factor
     */
    void applyGamma( float factor );
    
   /**
    * Normalizes the image.
    * This functions makes sure that every value of each pixel is between 0 and 1
    */
    void applyNormalize();
    
    /**
     * Inverts the Fourier transform.
     */
    void applyInverseFourier();
    
    void applyHighPass( float radius );
    
    void applyLowPass( float radius );
    
    void applyBandPass( float inRadius, float outRadius );
    
    void showErrorMessage( const char* message );
    
private:
    
    /** pointer to widget with the window */
    GtkWindow* _window;
    
    /** Original image object */
    Image* _srcImg;
    
    /** Edited image object */
    Image* _dstImg;
    
    /** Complex result of an eventual Fourier transform */
    std::complex<float>* _fourierImg;
};

#endif	/* MAINWINDOWPRESENTER_H */

