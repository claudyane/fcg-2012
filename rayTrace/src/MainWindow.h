/*
 * MainWindow.h
 *
 *  Created on: May 13, 2012
 *      Author: walther
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtk/gtk.h>

#include "MainWindowPresenter.h"

class MainWindow
{
public:
    MainWindow();
    virtual ~MainWindow();
    void show();

private:
    GtkWidget* build();
    GtkWidget* buildButtonsBox();
    GtkWidget* buildCanvasBox();
    GtkWidget* buildRayTraceCanvas();

    static gboolean cb_configGLCanvas( GtkWidget* canvas, GdkEventConfigure* event, gpointer user_data );
    static gboolean cb_exposeGLCanvas( GtkWidget* canvas, GdkEventExpose* event, gpointer user_data );
    static void cb_openScene( GtkWidget* button, gpointer user_data );
    static void cb_render( GtkWidget* button, gpointer user_data );

    GtkWidget* _window;
    GtkWidget* _rayTraceCanvas;
    MainWindowPresenter* _presenter;
};

#endif /* MAINWINDOW_H_ */
