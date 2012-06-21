/* 
 * File:   HistogramWindow.h
 * Author: eliana
 *
 * Created on June 21, 2012, 5:03 PM
 */

#ifndef HISTOGRAM_WINDOW_H
#define	HISTOGRAM_WINDOW_H

#include <gtk/gtk.h>
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
    
    GtkWidget* build();
    GtkWidget* buildCanvas();
    
    void beginGL();
    void endGL();
    
    static gboolean cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data );
    static gboolean cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data );
    static gboolean cb_deleteWindow( GtkWidget* widget, GdkEvent* event, gpointer data );
    
    Histogram* _histogram;
    int _width;
    int _height;
    
    GtkWidget* _window;
    GtkWidget* _canvas;
};

#endif	/* HISTOGRAM_WINDOW_H */

