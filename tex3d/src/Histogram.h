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
    Histogram();
    
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
    /** Where the values of the histogram will be stored */
    int _values[256];
};

#endif	/* HISTOGRAM_H */

