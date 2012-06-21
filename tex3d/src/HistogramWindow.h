/* 
 * File:   HistogramWindow.h
 * Author: eliana
 *
 * Created on June 21, 2012, 5:03 PM
 */

#ifndef HISTOGRAM_WINDOW_H
#define	HISTOGRAM_WINDOW_H

#include "Histogram.h"


class HistogramWindow
{
public:
    HistogramWindow();
    
    virtual ~HistogramWindow();
    
    void show();
    
    void hide();
    
    void setVolume( Volume* volume );
    
private:

    Histogram* _histogram;
};

#endif	/* HISTOGRAM_WINDOW_H */

