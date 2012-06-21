/* 
 * File:   Histogram.h
 * Author: walther
 *
 * Created on June 21, 2012, 4:55 PM
 */

#ifndef HISTOGRAM_H
#define	HISTOGRAM_H

#include "Volume.h"


class Histogram
{
public:
    /**
     * Default Constructor
     */
    Histogram( int width, int height );
    
    /**
     * Default Destructor
     */
    virtual ~Histogram();
    
    /**
     * Sets the volume it will represent
     */
    void setVolume( Volume* volume );
    
    /**
     * Calls a series of OpenGL functions to draw it.
     * This functions assumes there is a valid OpenGL context already active.
     */
    void draw();
    
private:
    /**
     * Cleans the _values vector, setting all to zero
     */
    void clear();
    
    void setupOGL();
    
    void cleanupOGL();
    
    /** Where the values of the histogram will be stored */
    int _values[256];
    
    /** Width of the available drawing area */
    int _width;
    
    /** Height of the available drawing area */
    int _height;
};

#endif	/* HISTOGRAM_H */

