/*
 * MainWindow.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtk/gtk.h>

class MainWindow
{
public:
    MainWindow();
    virtual ~MainWindow();

private:
    GtkWidget* build();
    GtkWidget* buildRayTraceCanvas();

    static gboolean cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data );
    static gboolean cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data );

    GtkWidget* _window;
    GtkWidget* _rayTraceCanvas;
};

#endif /* MAINWINDOW_H_ */
